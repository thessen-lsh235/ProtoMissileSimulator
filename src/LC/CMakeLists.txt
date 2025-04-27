cmake_minimum_required(VERSION 3.10)

project(LC_Project)

set(CMAKE_CXX_STANDARD 17)

# bin 폴더로 실행파일 모으기
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 공통 헤더파일
include_directories(${CMAKE_SOURCE_DIR}/../../common)

# 각 모듈(client, server, serial, logger) 추가
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(serial)
add_subdirectory(logger)

# [가장 중요] 실행파일(lc) 생성
add_executable(lc main.cpp)
add_executable(dummy_radar_server dummy_radar_server.cpp)
add_executable(dummy_radar_client dummy_radar_client.cpp)

# 실행파일에 모듈들 링크
target_link_libraries(lc
    lc_client
    lc_server
    lc_serial
    lc_logger
)
