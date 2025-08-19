#include "system_info.hpp"
#include <iostream>
#include <cassert>

void testSystemInfo() {
    std::cout << "SystemInfo 테스트 시작..." << std::endl;
    
    SystemInfo sys_info;
    
    // CPU 정보 테스트
    std::cout << "  CPU 정보 테스트..." << std::endl;
    auto cpu_info = sys_info.getCPUInfo();
    assert(!cpu_info.name.empty());
    assert(cpu_info.cores > 0);
    assert(cpu_info.threads > 0);
    assert(cpu_info.usage_percent >= 0.0 && cpu_info.usage_percent <= 100.0);
    
    // 메모리 정보 테스트
    std::cout << "  메모리 정보 테스트..." << std::endl;
    auto mem_info = sys_info.getMemoryInfo();
    assert(mem_info.total_bytes > 0);
    assert(mem_info.available_bytes > 0);
    assert(mem_info.used_bytes >= 0);
    assert(mem_info.usage_percent >= 0.0 && mem_info.usage_percent <= 100.0);
    
    // 디스크 정보 테스트
    std::cout << "  디스크 정보 테스트..." << std::endl;
    auto disk_info = sys_info.getDiskInfo();
    assert(!disk_info.empty());
    for (const auto& disk : disk_info) {
        assert(!disk.name.empty());
        assert(disk.total_bytes > 0);
        assert(disk.used_bytes >= 0);
        assert(disk.free_bytes >= 0);
        assert(disk.usage_percent >= 0.0 && disk.usage_percent <= 100.0);
    }
    
    // 네트워크 정보 테스트
    std::cout << "  네트워크 정보 테스트..." << std::endl;
    auto network_info = sys_info.getNetworkInfo();
    // 네트워크 인터페이스가 없을 수도 있음
    for (const auto& net : network_info) {
        assert(!net.interface_name.empty());
    }
    
    // 시스템 부하 테스트
    std::cout << "  시스템 부하 테스트..." << std::endl;
    auto load = sys_info.getSystemLoad();
    assert(load >= 0.0);
    
    // 업타임 테스트
    std::cout << "  업타임 테스트..." << std::endl;
    auto uptime = sys_info.getUptime();
    assert(!uptime.empty());
    
    std::cout << "  SystemInfo 테스트 완료!" << std::endl;
}
