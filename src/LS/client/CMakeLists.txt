add_library(udp_launcher STATIC udp_launcher.cpp)

# common 헤더 사용
target_include_directories(udp_launcher PRIVATE ${CMAKE_SOURCE_DIR}/../common)

# 공용 common 라이브러리 링크
target_link_libraries(udp_launcher PRIVATE common)

add_executable(uart_launchercontrol uart_launchercontrol.cpp)

target_include_directories(uart_launchercontrol PRIVATE ${CMAKE_SOURCE_DIR}/../common)
target_link_libraries(uart_launchercontrol PRIVATE common)
