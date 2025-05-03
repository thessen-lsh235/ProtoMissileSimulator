cmake_minimum_required(VERSION 3.10)
project(LauncherSystem)

set(CMAKE_CXX_STANDARD 17)

# ------------------------------------------------------------
# 📁 Include directories (상대경로 기준)
# 모든 타겟이 공통 헤더를 사용할 수 있도록 include 등록
# ------------------------------------------------------------
include_directories(
    ${CMAKE_SOURCE_DIR}/../common
    ${CMAKE_SOURCE_DIR}/../common/ini_parser
    common                         # launcher_config_handler.h (LS 전용 핸들러)
)

# ------------------------------------------------------------
# 📦 공통 라이브러리 (common)
# launcher_config, ini_parser 등 공용 기능을 포함
# ------------------------------------------------------------
add_library(common
    ../common/ini_parser/ini_parser.cpp
)

# ------------------------------------------------------------
# 📦 LS 전용 이벤트 핸들러 라이브러리 (launcher_handler)
# launcher_config_handler.cpp을 별도 링크 가능하도록 구성
# ------------------------------------------------------------
add_library(launcher_handler
    common/launcher_config_handler.cpp
)
target_link_libraries(launcher_handler PRIVATE common)

# ------------------------------------------------------------
# 🚀 Client (udp_launcher) 라이브러리 빌드
# ------------------------------------------------------------
add_subdirectory(client)

# ------------------------------------------------------------
# 🛰️ Server 실행파일 (uart_server)
# ------------------------------------------------------------
add_subdirectory(server)

# ------------------------------------------------------------
# 💻 temp_LC 실행파일 (임시 발사통제기)
# ------------------------------------------------------------
add_executable(temp_LC_launchercontrol
    temp_LC/uart_launchercontrol.cpp
)
target_link_libraries(temp_LC_launchercontrol PRIVATE common)
target_include_directories(temp_LC_launchercontrol PRIVATE
    ../common
    ../common/ini_parser
    common
)

# ------------------------------------------------------------
# 🧪 temp_SIM 실행파일 (임시 시뮬레이터)
# ------------------------------------------------------------
add_executable(temp_SIM_udpserver
    temp_SIM/udp_server.cpp
)
target_link_libraries(temp_SIM_udpserver PRIVATE common)
target_include_directories(temp_SIM_udpserver PRIVATE
    ../common
    ../common/ini_parser
    common
)

# ------------------------------------------------------------
# 🧩 launcher_main 실행파일 (통합 메인)
# ------------------------------------------------------------
add_executable(launcher_main
    main.cpp
    server/uart_server.cpp
    ../common/ini_parser/ini_parser.cpp
)
target_include_directories(launcher_main PRIVATE
    ../common
    ../common/ini_parser
    common
    client
    server
)
target_link_libraries(launcher_main
    PRIVATE
        common
        launcher_handler
        udp_launcher
        uart_server
)
