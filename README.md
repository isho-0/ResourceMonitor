# 🖥️ C++ 하드웨어 리소스 모니터

![Language](https://img.shields.io/badge/language-C++17-blue?logo=c%2B%2B)
![Build](https://img.shields.io/badge/build-CMake-green?logo=cmake)
![Platform](https://img.shields.io/badge/platform-macOS%20%7C%20Linux%20%7C%20Windows-orange?logo=apple)
![Status](https://img.shields.io/badge/status-production%20ready-brightgreen)

> "C++17로 만든 크로스 플랫폼 하드웨어 리소스 모니터링 도구"  
> 실시간으로 시스템 성능을 모니터링하며, 개발자와 파워 유저에게 최적의 시스템 관리 경험을 제공합니다.

---

## 🧭 개요

이 프로젝트는 **C++17**을 사용하여 개발된 크로스 플랫폼 하드웨어 리소스 모니터링 프로그램입니다.  
CPU, 메모리, 디스크, 네트워크 등 시스템의 핵심 리소스를 실시간으로 추적하고 시각화하여,  
시스템 성능 최적화와 문제 진단에 필요한 정보를 제공합니다.

---

## ⚙️ 환경

- **언어**: C++17
- **빌드 시스템**: CMake 3.16+
- **지원 플랫폼**: macOS, Linux, Windows
- **컴파일러**: GCC, Clang, MSVC
- **버전 관리**: Git & GitHub

---

## 🗂️ 프로젝트 구조

<pre>
ResourceMonitor/
├── CMakeLists.txt          # 메인 CMake 설정
├── src/                    # 소스 코드
│   ├── CMakeLists.txt      # 소스 빌드 설정
│   ├── main.cpp            # 메인 함수
│   ├── resource_monitor.hpp # 리소스 모니터 클래스
│   ├── resource_monitor.cpp
│   ├── system_info.hpp     # 시스템 정보 클래스
│   └── system_info.cpp
├── tests/                  # 테스트 코드
│   ├── CMakeLists.txt      # 테스트 빌드 설정
│   ├── main.cpp            # 테스트 메인
│   ├── system_info_test.cpp
│   └── resource_monitor_test.cpp
├── .gitignore              # Git 무시 파일
├── .clang-format           # 코드 포맷팅 설정
└── README.md               # 프로젝트 문서
</pre>

---

## 🚀 주요 기능

### 🔥 **실시간 모니터링**
- **CPU 모니터링**: 사용률, 코어 수, 스레드 수, 모델명
- **메모리 모니터링**: 총 메모리, 사용량, 사용률, 가용 메모리
- **디스크 모니터링**: 용량, 사용량, 사용률, 파일시스템 정보
- **네트워크 모니터링**: 인터페이스 목록, 연결 상태
- **시스템 정보**: 업타임, 시스템 부하, 부팅 시간

### ⚡ **고급 기능**
- **설정 가능한 업데이트 간격**: 1초~무제한
- **콜백 함수 지원**: 사용자 정의 처리 로직
- **멀티스레딩**: 비동기 모니터링으로 시스템 영향 최소화
- **크로스 플랫폼**: macOS, Linux, Windows 완벽 지원
- **실시간 화면 갱신**: 터미널 기반 깔끔한 UI

---

## 📦 빌드 및 실행

### **1. 저장소 클론**
```bash
git clone <repository-url>
cd ResourceMonitor
```

### **2. 빌드**
```bash
mkdir build && cd build
cmake ..
make
```

### **3. 실행**
```bash
./src/ResourceMonitor
```

### **4. 테스트 실행**
```bash
cmake -DBUILD_TESTS=ON ..
make
./tests/ResourceMonitorTests
```

---

## 💻 사용법

### **기본 사용**
```cpp
#include "resource_monitor.hpp"

int main() {
    ResourceMonitor monitor;
    
    // 1초마다 업데이트
    monitor.setUpdateInterval(1000);
    
    // 모니터링 시작
    monitor.start();
    
    // ... 프로그램 실행 ...
    
    // 모니터링 중지
    monitor.stop();
    
    return 0;
}
```

### **콜백 함수 사용**
```cpp
monitor.setUpdateCallback([](const SystemInfo& info) {
    // 시스템 정보가 업데이트될 때마다 호출됨
    auto cpu = info.getCPUInfo();
    std::cout << "CPU 사용률: " << cpu.usage_percent << "%" << std::endl;
});
```

---

## 🔧 플랫폼별 구현

### **macOS**
- **IOKit, CoreFoundation, SystemConfiguration** 프레임워크
- **Mach 커널 API**를 통한 시스템 정보 수집
- **sysctl** 시스템 콜 활용

### **Linux**
- **/proc** 파일시스템을 통한 시스템 정보 수집
- **sysinfo** 시스템 콜 사용
- **statvfs**를 통한 디스크 정보

### **Windows**
- **Performance Data Helper (PDH)** API
- **Process Status API (PSAPI)** 사용
- **Windows Management Instrumentation (WMI)** 지원

---

## 🧪 테스트

프로젝트는 포괄적인 테스트 스위트를 포함합니다:

- **SystemInfo 테스트**: 시스템 정보 수집 기능 검증
- **ResourceMonitor 테스트**: 모니터링 기능 검증
- **크로스 플랫폼 테스트**: 다양한 OS 환경에서 동작 확인

---

## 📊 성능 및 정확도

### **Activity Monitor와의 비교**
- **CPU 사용률**: 기본적인 틱 기반 계산 (간단하지만 안정적)
- **메모리 정보**: 물리 메모리 중심 (세부 타입 구분은 향후 개선 예정)
- **업데이트 주기**: 1초 기본 (설정 가능)
- **시스템 영향**: 최소한의 리소스 사용

### **정확도 개선 계획**
- [ ] 더 정교한 CPU 사용률 계산 알고리즘
- [ ] 세부 메모리 타입 구분 (App, Wired, Cached, Swap)
- [ ] 프로세스별 리소스 사용량 모니터링
- [ ] GPU 사용률 모니터링 추가

---

## 🌊 Commit Convention (커밋 규칙)

| 유형 | 설명 |
|------|------|
| `feat` | 새로운 기능 추가 |
| `fix` | 버그 수정 |
| `docs` | 문서 수정 |
| `refactor` | 코드 리팩토링 |
| `test` | 테스트 추가 또는 수정 |
| `perf` | 성능 개선 |
| `build` | 빌드 시스템 또는 외부 종속성 변경 |

**예시**:
```bash
git commit -m "feat: CPU 모델명 자동 감지 기능 추가"
git commit -m "fix: 메모리 사용률 계산 오류 수정"
git commit -m "docs: README 업데이트 및 사용법 추가"
```

---

## 🤝 기여하기

버그 리포트, 기능 요청, 풀 리퀘스트를 환영합니다!

### **기여 방법**
1. 이슈 생성 또는 기존 이슈 확인
2. 포크 후 기능 브랜치 생성
3. 코드 작성 및 테스트
4. 풀 리퀘스트 제출

### **개발 환경 설정**
```bash
# 개발 의존성 설치
brew install cmake  # macOS
sudo apt install cmake  # Ubuntu/Debian

# 코드 포맷팅
clang-format -i src/*.cpp src/*.hpp
```

---

## 📄 라이선스

이 프로젝트는 **MIT 라이선스** 하에 배포됩니다.

---

## 🙋‍♂️ About Me

- **GitHub**: [@isho-0](https://github.com/isho-0)
- **프로젝트**: C++ 하드웨어 리소스 모니터
- **목표**: 크로스 플랫폼 시스템 모니터링 도구 개발

---

**Made with ❤️ by isho-0.**  
*실시간으로 시스템을 모니터링하며, 최적의 성능을 추구합니다.*
