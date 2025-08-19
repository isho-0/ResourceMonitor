# 소스 파일들
set(SOURCES
    main.cpp
    resource_monitor.cpp
    system_info.cpp
)

set(HEADERS
    resource_monitor.hpp
    system_info.hpp
)

# 실행 파일 생성
add_executable(ResourceMonitor ${SOURCES} ${HEADERS})

# 플랫폼별 라이브러리 링크
if(APPLE)
    target_link_libraries(ResourceMonitor 
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework SystemConfiguration"
    )
elseif(UNIX AND NOT APPLE)
    target_link_libraries(ResourceMonitor 
        ${CMAKE_DL_LIBS}
    )
elseif(WIN32)
    target_link_libraries(ResourceMonitor 
        pdh.lib
        psapi.lib
    )
endif()

# 헤더 파일 경로 설정
target_include_directories(ResourceMonitor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
