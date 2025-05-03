add_library(uart_server STATIC uart_server.cpp)

target_include_directories(uart_server PRIVATE
    ${CMAKE_SOURCE_DIR}/../common
    ${CMAKE_SOURCE_DIR}/../common/ini_parser
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/client   
)


target_link_libraries(uart_server
    PRIVATE
        common
        launcher_handler
        udp_launcher
)