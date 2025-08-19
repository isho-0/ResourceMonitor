#include "resource_monitor.hpp"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

ResourceMonitor::ResourceMonitor() 
    : running_(false)
    , update_interval_ms_(1000) {
}

ResourceMonitor::~ResourceMonitor() {
    stop();
}

void ResourceMonitor::start() {
    if (running_) return;
    
    running_ = true;
    monitor_thread_ = std::thread(&ResourceMonitor::monitoringLoop, this);
    std::cout << "리소스 모니터링이 시작되었습니다. (업데이트 간격: " << update_interval_ms_ << "ms)" << std::endl;
}

void ResourceMonitor::stop() {
    if (!running_) return;
    
    running_ = false;
    if (monitor_thread_.joinable()) {
        monitor_thread_.join();
    }
    std::cout << "리소스 모니터링이 중지되었습니다." << std::endl;
}

bool ResourceMonitor::isRunning() const {
    return running_;
}

void ResourceMonitor::setUpdateCallback(std::function<void(const SystemInfo&)> callback) {
    update_callback_ = callback;
}

void ResourceMonitor::setUpdateInterval(int milliseconds) {
    update_interval_ms_ = milliseconds;
}

SystemInfo ResourceMonitor::getCurrentSystemInfo() const {
    return system_info_;
}

void ResourceMonitor::monitoringLoop() {
    while (running_) {
        auto start_time = std::chrono::steady_clock::now();
        
        // 시스템 정보 업데이트
        system_info_ = SystemInfo();
        
        // 콜백 함수 호출
        if (update_callback_) {
            update_callback_(system_info_);
        }
        
        // 시스템 정보 출력
        printSystemInfo();
        
        // 업데이트 간격만큼 대기
        auto elapsed = std::chrono::steady_clock::now() - start_time;
        auto sleep_time = std::chrono::milliseconds(update_interval_ms_) - elapsed;
        
        if (sleep_time > std::chrono::milliseconds(0)) {
            std::this_thread::sleep_for(sleep_time);
        }
    }
}

void ResourceMonitor::printSystemInfo() const {
    const auto& cpu = system_info_.getCPUInfo();
    const auto& mem = system_info_.getMemoryInfo();
    const auto& disks = system_info_.getDiskInfo();
    const auto& networks = system_info_.getNetworkInfo();
    
    std::cout << "\033[2J\033[H"; // 화면 클리어
    std::cout << "=== 시스템 리소스 모니터 ===" << std::endl;
    std::cout << "업타임: " << system_info_.getUptime() << std::endl;
    std::cout << "시스템 부하: " << std::fixed << std::setprecision(2) << system_info_.getSystemLoad() << std::endl;
    std::cout << std::endl;
    
    // CPU 정보
    std::cout << "CPU 정보:" << std::endl;
    std::cout << "  이름: " << cpu.name << std::endl;
    std::cout << "  코어: " << cpu.cores << std::endl;
    std::cout << "  스레드: " << cpu.threads << std::endl;
    std::cout << "  사용률: " << std::fixed << std::setprecision(1) << cpu.usage_percent << "%" << std::endl;
    std::cout << std::endl;
    
    // 메모리 정보
    std::cout << "메모리 정보:" << std::endl;
    std::cout << "  총 메모리: " << (mem.total_bytes / (1024 * 1024 * 1024)) << " GB" << std::endl;
    std::cout << "  사용 중: " << (mem.used_bytes / (1024 * 1024 * 1024)) << " GB" << std::endl;
    std::cout << "  사용 가능: " << (mem.available_bytes / (1024 * 1024 * 1024)) << " GB" << std::endl;
    std::cout << "  사용률: " << std::fixed << std::setprecision(1) << mem.usage_percent << "%" << std::endl;
    std::cout << std::endl;
    
    // 디스크 정보
    if (!disks.empty()) {
        std::cout << "디스크 정보:" << std::endl;
        for (const auto& disk : disks) {
            std::cout << "  " << disk.name << ":" << std::endl;
            std::cout << "    총 용량: " << (disk.total_bytes / (1024 * 1024 * 1024)) << " GB" << std::endl;
            std::cout << "    사용 중: " << (disk.used_bytes / (1024 * 1024 * 1024)) << " GB" << std::endl;
            std::cout << "    사용률: " << std::fixed << std::setprecision(1) << disk.usage_percent << "%" << std::endl;
        }
        std::cout << std::endl;
    }
    
    // 네트워크 정보
    if (!networks.empty()) {
        std::cout << "네트워크 인터페이스:" << std::endl;
        for (const auto& net : networks) {
            std::cout << "  " << net.interface_name << std::endl;
        }
        std::cout << std::endl;
    }
    
    // 현재 시간을 년월일시분초로 변환
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    std::cout << "업데이트 시간: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << std::endl;
}
