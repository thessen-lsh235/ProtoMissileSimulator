# client/CMakeLists.txt
add_executable(udp_launcher udp_launcher.cpp ini.c)
target_include_directories(udp_launcher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(udp_launcher PRIVATE common)
