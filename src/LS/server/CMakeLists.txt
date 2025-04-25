# server/CMakeLists.txt
add_executable(udp_server udp_server.cpp)
target_link_libraries(udp_server PRIVATE common)
