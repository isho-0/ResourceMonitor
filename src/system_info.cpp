#include "system_info.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#ifdef __APPLE__
#include <mach/mach.h>
#include <mach/mach_host.h>
#include <sys/sysctl.h>
#include <sys/statvfs.h>
#include <net/if.h>
#include <ifaddrs.h>
#elif defined(__linux__)
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <fstream>
#include <dirent.h>
#elif defined(_WIN32)
#include <windows.h>
#include <pdh.h>
#include <psapi.h>
#endif

SystemInfo::SystemInfo() {
    updateCPUInfo();
    updateMemoryInfo();
    updateDiskInfo();
    updateNetworkInfo();
}

SystemInfo::~SystemInfo() = default;

CPUInfo SystemInfo::getCPUInfo() const {
    const_cast<SystemInfo*>(this)->updateCPUInfo();
    return cpu_info_;
}

MemoryInfo SystemInfo::getMemoryInfo() const {
    const_cast<SystemInfo*>(this)->updateMemoryInfo();
    return memory_info_;
}

std::vector<DiskInfo> SystemInfo::getDiskInfo() const {
    const_cast<SystemInfo*>(this)->updateDiskInfo();
    return disk_info_;
}

std::vector<NetworkInfo> SystemInfo::getNetworkInfo() const {
    const_cast<SystemInfo*>(this)->updateNetworkInfo();
    return network_info_;
}

double SystemInfo::getSystemLoad() const {
#ifdef __APPLE__
    double loadavg[3];
    if (getloadavg(loadavg, 3) != -1) {
        return loadavg[0]; // 1분 평균
    }
#elif defined(__linux__)
    double loadavg[3];
    if (getloadavg(loadavg, 3) != -1) {
        return loadavg[0]; // 1분 평균
    }
#endif
    return 0.0;
}

std::string SystemInfo::getUptime() const {
#ifdef __APPLE__
    struct timeval boottime;
    size_t len = sizeof(boottime);
    if (sysctlbyname("kern.boottime", &boottime, &len, nullptr, 0) == 0) {
        time_t uptime = time(nullptr) - boottime.tv_sec;
        int days = uptime / 86400;
        int hours = (uptime % 86400) / 3600;
        int minutes = (uptime % 3600) / 60;
        return std::to_string(days) + "d " + std::to_string(hours) + "h " + std::to_string(minutes) + "m";
    }
#elif defined(__linux__)
    std::ifstream uptime_file("/proc/uptime");
    if (uptime_file.is_open()) {
        double uptime_seconds;
        uptime_file >> uptime_seconds;
        int days = static_cast<int>(uptime_seconds) / 86400;
        int hours = (static_cast<int>(uptime_seconds) % 86400) / 3600;
        int minutes = (static_cast<int>(uptime_seconds) % 3600) / 60;
        return std::to_string(days) + "d " + std::to_string(hours) + "h " + std::to_string(minutes) + "m";
    }
#endif
    return "Unknown";
}

void SystemInfo::updateCPUInfo() {
#ifdef __APPLE__
    // macOS에서 CPU 정보 가져오기
    int cores = 0;
    size_t len = sizeof(cores);
    if (sysctlbyname("hw.ncpu", &cores, &len, nullptr, 0) == 0) {
        cpu_info_.cores = cores;
        cpu_info_.threads = cores;
    }
    
    // CPU 사용률 계산 (간단한 구현)
    static uint64_t last_total = 0, last_idle = 0;
    uint64_t total = 0, idle = 0;
    
    host_cpu_load_info_data_t cpu_load;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpu_load, &count) == KERN_SUCCESS) {
        total = cpu_load.cpu_ticks[CPU_STATE_USER] + cpu_load.cpu_ticks[CPU_STATE_SYSTEM] + 
                cpu_load.cpu_ticks[CPU_STATE_IDLE] + cpu_load.cpu_ticks[CPU_STATE_NICE];
        idle = cpu_load.cpu_ticks[CPU_STATE_IDLE];
        
        if (last_total > 0) {
            uint64_t total_diff = total - last_total;
            uint64_t idle_diff = idle - last_idle;
            if (total_diff > 0) {
                cpu_info_.usage_percent = 100.0 * (1.0 - static_cast<double>(idle_diff) / total_diff);
            }
        }
        
        last_total = total;
        last_idle = idle;
    }
    
    // CPU 모델명 가져오기
    char cpu_model[256];
    size_t model_len = sizeof(cpu_model);
    if (sysctlbyname("machdep.cpu.brand_string", cpu_model, &model_len, nullptr, 0) == 0) {
        cpu_info_.name = cpu_model;
    } else {
        cpu_info_.name = "Apple Silicon / Intel";
    }
    cpu_info_.temperature = 0.0; // macOS에서는 직접적인 온도 정보 접근이 제한적
#endif
}

void SystemInfo::updateMemoryInfo() {
#ifdef __APPLE__
    vm_size_t page_size;
    mach_port_t host = mach_host_self();
    mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
    vm_statistics_data_t vm_stats;
    
    if (host_page_size(host, &page_size) == KERN_SUCCESS &&
        host_statistics(host, HOST_VM_INFO, (host_info_t)&vm_stats, &count) == KERN_SUCCESS) {
        
        uint64_t total_mem = 0;
        size_t len = sizeof(total_mem);
        sysctlbyname("hw.memsize", &total_mem, &len, nullptr, 0);
        
        memory_info_.total_bytes = total_mem;
        memory_info_.available_bytes = (vm_stats.free_count + vm_stats.inactive_count) * page_size;
        memory_info_.used_bytes = total_mem - memory_info_.available_bytes;
        memory_info_.usage_percent = 100.0 * static_cast<double>(memory_info_.used_bytes) / total_mem;
    }
#endif
}

void SystemInfo::updateDiskInfo() {
    disk_info_.clear();
    
#ifdef __APPLE__
    // macOS에서 디스크 정보 가져오기
    struct statvfs stat;
    if (statvfs("/", &stat) == 0) {
        DiskInfo disk;
        disk.name = "/";
        disk.total_bytes = stat.f_blocks * stat.f_frsize;
        disk.free_bytes = stat.f_bavail * stat.f_frsize;
        disk.used_bytes = disk.total_bytes - disk.free_bytes;
        disk.usage_percent = 100.0 * static_cast<double>(disk.used_bytes) / disk.total_bytes;
        disk_info_.push_back(disk);
    }
#endif
}

void SystemInfo::updateNetworkInfo() {
    network_info_.clear();
    
#ifdef __APPLE__
    // macOS에서 네트워크 정보 가져오기
    struct ifaddrs *ifaddr;
    if (getifaddrs(&ifaddr) == 0) {
        for (struct ifaddrs *ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr == nullptr) continue;
            
            if (ifa->ifa_addr->sa_family == AF_LINK) {
                NetworkInfo net;
                net.interface_name = ifa->ifa_name;
                net.bytes_sent = 0;
                net.bytes_received = 0;
                net.bandwidth_mbps = 0.0;
                network_info_.push_back(net);
            }
        }
        freeifaddrs(ifaddr);
    }
#endif
}
