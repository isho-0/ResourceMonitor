#pragma once

#include <string>
#include <vector>
#include <cstdint>

struct CPUInfo {
    std::string name;
    int cores;
    int threads;
    double usage_percent;
    double temperature;
};

struct MemoryInfo {
    uint64_t total_bytes;
    uint64_t available_bytes;
    uint64_t used_bytes;
    double usage_percent;
};

struct DiskInfo {
    std::string name;
    uint64_t total_bytes;
    uint64_t used_bytes;
    uint64_t free_bytes;
    double usage_percent;
};

struct NetworkInfo {
    std::string interface_name;
    uint64_t bytes_sent;
    uint64_t bytes_received;
    double bandwidth_mbps;
};

class SystemInfo {
public:
    SystemInfo();
    ~SystemInfo();
    
    // CPU 정보 가져오기
    CPUInfo getCPUInfo() const;
    
    // 메모리 정보 가져오기
    MemoryInfo getMemoryInfo() const;
    
    // 디스크 정보 가져오기
    std::vector<DiskInfo> getDiskInfo() const;
    
    // 네트워크 정보 가져오기
    std::vector<NetworkInfo> getNetworkInfo() const;
    
    // 시스템 부하 정보
    double getSystemLoad() const;
    
    // 업타임 정보
    std::string getUptime() const;

private:
    // 플랫폼별 구현
    void updateCPUInfo();
    void updateMemoryInfo();
    void updateDiskInfo();
    void updateNetworkInfo();
    
    // 내부 데이터
    CPUInfo cpu_info_;
    MemoryInfo memory_info_;
    std::vector<DiskInfo> disk_info_;
    std::vector<NetworkInfo> network_info_;
};
