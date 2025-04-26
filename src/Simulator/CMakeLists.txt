cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름 및 C++ 표준 설정
project(ProtoMissileSimulator)
set(CMAKE_CXX_STANDARD 17)

# 디버그 심볼 활성화
set(CMAKE_BUILD_TYPE Debug)

# fmt 라이브러리 추가
find_package(fmt REQUIRED)

# 소스 파일 목록
set(SOURCES
    main.cpp
    UDPServer.cpp
)

# 실행 파일 생성
add_executable(ProtoMissileSimulator ${SOURCES})

# common 및 fmt 라이브러리 링크
target_link_libraries(ProtoMissileSimulator PRIVATE common fmt::fmt)

# common 헤더 파일 경로 추가
target_include_directories(ProtoMissileSimulator PRIVATE ${CMAKE_SOURCE_DIR}/../common)

# test_client 서브 디렉토리 추가
add_subdirectory(test_client)
