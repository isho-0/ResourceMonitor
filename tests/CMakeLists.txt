# 테스트 실행 파일 생성
add_executable(ResourceMonitorTests
    main.cpp
    system_info_test.cpp
    resource_monitor_test.cpp
)

# 소스 파일들을 직접 컴파일
target_sources(ResourceMonitorTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/system_info.cpp
    ${CMAKE_SOURCE_DIR}/src/resource_monitor.cpp
)

# 헤더 파일 경로 설정
target_include_directories(ResourceMonitorTests PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
)

# 테스트 추가
add_test(NAME ResourceMonitorTests COMMAND ResourceMonitorTests)
