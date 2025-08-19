#pragma once

#include "system_info.hpp"
#include <chrono>
#include <thread>
#include <atomic>
#include <functional>

class ResourceMonitor {
public:
    ResourceMonitor();
    ~ResourceMonitor();
    
    // 모니터링 시작
    void start();
    
    // 모니터링 중지
    void stop();
    
    // 모니터링 상태 확인
    bool isRunning() const;
    
    // 콜백 함수 설정
    void setUpdateCallback(std::function<void(const SystemInfo&)> callback);
    
    // 업데이트 간격 설정 (밀리초)
    void setUpdateInterval(int milliseconds);
    
    // 현재 시스템 정보 가져오기
    SystemInfo getCurrentSystemInfo() const;

private:
    // 모니터링 루프
    void monitoringLoop();
    
    // 데이터 출력
    void printSystemInfo() const;
    
    // 내부 변수들
    std::atomic<bool> running_;
    std::thread monitor_thread_;
    SystemInfo system_info_;
    std::function<void(const SystemInfo&)> update_callback_;
    int update_interval_ms_;
    
    // 시간 관련
    std::chrono::steady_clock::time_point last_update_;
};
