cmake_minimum_required(VERSION 3.10)
project(LauncherProject)

# C++17 사용
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 상위 common 디렉토리 추가 (라이브러리 + 헤더)
add_subdirectory(${CMAKE_SOURCE_DIR}/../common ${CMAKE_BINARY_DIR}/common_build)

# 하위 client, server 디렉토리 추가
add_subdirectory(client)
add_subdirectory(server)
