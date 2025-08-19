#include "resource_monitor.hpp"
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>

void testResourceMonitor() {
    std::cout << "ResourceMonitor 테스트 시작..." << std::endl;
    
    ResourceMonitor monitor;
    
    // 초기 상태 테스트
    std::cout << "  초기 상태 테스트..." << std::endl;
    assert(!monitor.isRunning());
    
    // 업데이트 간격 설정 테스트
    std::cout << "  업데이트 간격 설정 테스트..." << std::endl;
    monitor.setUpdateInterval(500);
    
    // 콜백 함수 설정 테스트
    std::cout << "  콜백 함수 설정 테스트..." << std::endl;
    bool callback_called = false;
    monitor.setUpdateCallback([&callback_called](const SystemInfo& info) {
        callback_called = true;
        // 콜백이 호출되었는지 확인
        assert(!info.getCPUInfo().name.empty());
    });
    
    // 모니터링 시작 테스트
    std::cout << "  모니터링 시작 테스트..." << std::endl;
    monitor.start();
    assert(monitor.isRunning());
    
    // 잠시 대기하여 콜백이 호출되는지 확인
    std::cout << "  콜백 호출 대기..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(600));
    
    // 콜백이 호출되었는지 확인
    assert(callback_called);
    
    // 현재 시스템 정보 가져오기 테스트
    std::cout << "  시스템 정보 가져오기 테스트..." << std::endl;
    auto current_info = monitor.getCurrentSystemInfo();
    assert(!current_info.getCPUInfo().name.empty());
    
    // 모니터링 중지 테스트
    std::cout << "  모니터링 중지 테스트..." << std::endl;
    monitor.stop();
    assert(!monitor.isRunning());
    
    std::cout << "  ResourceMonitor 테스트 완료!" << std::endl;
}
