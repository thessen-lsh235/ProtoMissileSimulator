add_executable(udp_server udp_server.cpp)

target_include_directories(udp_server PRIVATE ${CMAKE_SOURCE_DIR}/../common)

target_link_libraries(udp_server PRIVATE common)
