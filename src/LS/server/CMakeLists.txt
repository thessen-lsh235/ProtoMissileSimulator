# 서버 실행 파일 두 개 생성
add_executable(udp_server udp_server.cpp)
add_executable(uart_server uart_server.cpp ini.c)

# udp_server 설정
target_include_directories(udp_server PRIVATE ${CMAKE_SOURCE_DIR}/../common)
target_link_libraries(udp_server PRIVATE common)

# uart_server 설정
target_include_directories(uart_server PRIVATE ${CMAKE_SOURCE_DIR}/../common)
target_link_libraries(uart_server PRIVATE udp_launcher)
target_link_libraries(uart_server PRIVATE common)
