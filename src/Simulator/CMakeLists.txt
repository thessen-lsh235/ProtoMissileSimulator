cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름 및 C++ 표준 설정
project(ProtoMissileSimulator)
set(CMAKE_CXX_STANDARD 17)

# 소스 파일 목록
set(SOURCES
    main.cpp
    # Entities.cpp
    # EntityFactory.cpp
    UDPServer.cpp
    util.cpp
)

# 실행 파일 생성
add_executable(ProtoMissileSimulator ${SOURCES})
add_subdirectory(test_client)
