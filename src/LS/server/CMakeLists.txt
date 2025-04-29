add_executable(uart_server uart_server.cpp)

target_include_directories(uart_server PRIVATE
    ../common
    ../common/ini_parser
    ../client 
)

target_link_libraries(uart_server PRIVATE common)
