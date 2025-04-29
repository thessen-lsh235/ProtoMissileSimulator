cmake_minimum_required(VERSION 3.10)
project(LauncherSystem)

set(CMAKE_CXX_STANDARD 17)

# Include directories (상대경로 기준)
include_directories(
    ../common                      # missileInfo.h, type.h
    ../common/ini_parser           # ini_parser.h
    common                         # launcher_config.h, launch_protocol.h
)

# 공통 라이브러리: launcher_config.cpp만 포함
add_library(common
    common/launcher_config.cpp
    ../common/ini_parser/ini_parser.cpp
)

# Client 라이브러리 빌드 (udp_launcher)
add_subdirectory(client)

# Server 실행파일 빌드 (uart_server)
add_subdirectory(server)

# temp_LC (임시 발사통제기 실행파일)
add_executable(temp_LC_launchercontrol
    temp_LC/uart_launchercontrol.cpp
)
target_link_libraries(temp_LC_launchercontrol PRIVATE common)
target_include_directories(temp_LC_launchercontrol PRIVATE
    ../common
    ../common/ini_parser
    common
)

# temp_SIM (임시 시뮬레이터 실행파일)
add_executable(temp_SIM_udpserver
    temp_SIM/udp_server.cpp
)
target_link_libraries(temp_SIM_udpserver PRIVATE common)
target_include_directories(temp_SIM_udpserver PRIVATE
    ../common
    ../common/ini_parser
    common
)

# launcher_main 실행파일 (메인 통합 실행)
add_executable(launcher_main
    main.cpp
    server/uart_server.cpp                  # runUartServer() 구현
    ../common/ini_parser/ini_parser.cpp    # IniParser::parse() 등 구현
)
target_link_libraries(launcher_main PRIVATE common udp_launcher)
target_include_directories(launcher_main PRIVATE
    ../common
    ../common/ini_parser
    common
    client
    server
)
