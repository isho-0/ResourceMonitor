#include "resource_monitor.hpp"
#include <iostream>
#include <signal.h>
#include <thread>
#include <chrono>

std::atomic<bool> running(true);

void signalHandler(int signum) {
    std::cout << "\n시그널 " << signum << "을 받았습니다. 프로그램을 종료합니다..." << std::endl;
    running = false;
}

int main() {
    std::cout << "=== C++ 하드웨어 리소스 모니터 ===" << std::endl;
    std::cout << "버전: 1.0.0" << std::endl;
    std::cout << "C++17 표준 사용" << std::endl;
    std::cout << std::endl;
    
    // 시그널 핸들러 설정
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    try {
        ResourceMonitor monitor;
        
        // 업데이트 간격 설정 (1초)
        monitor.setUpdateInterval(1000);
        
        // 콜백 함수 설정 (선택사항)
        monitor.setUpdateCallback([](const SystemInfo& info) {
            // 여기에 추가적인 처리 로직을 추가할 수 있습니다
            // 예: 로그 파일 저장, 네트워크 전송 등
        });
        
        // 모니터링 시작
        monitor.start();
        
        std::cout << "프로그램이 실행 중입니다. Ctrl+C를 눌러 종료하세요." << std::endl;
        
        // 메인 루프
        while (running && monitor.isRunning()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        // 모니터링 중지
        monitor.stop();
        
    } catch (const std::exception& e) {
        std::cerr << "오류가 발생했습니다: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "알 수 없는 오류가 발생했습니다." << std::endl;
        return 1;
    }
    
    std::cout << "프로그램이 정상적으로 종료되었습니다." << std::endl;
    return 0;
}
