cmake_minimum_required(VERSION 3.10)
project(LauncherProject)

# C++17 사용
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 서브 디렉토리 추가
add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
