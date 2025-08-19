#include <iostream>
#include <cassert>

// 테스트 함수들 선언
void testSystemInfo();
void testResourceMonitor();

int main() {
    std::cout << "=== 리소스 모니터 테스트 시작 ===" << std::endl;
    
    try {
        testSystemInfo();
        testResourceMonitor();
        
        std::cout << "모든 테스트가 성공적으로 완료되었습니다!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "테스트 실패: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "알 수 없는 테스트 오류가 발생했습니다." << std::endl;
        return 1;
    }
}
