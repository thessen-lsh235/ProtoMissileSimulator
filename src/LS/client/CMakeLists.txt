# client/CMakeLists.txt
add_library(udp_launcher
    udp_launcher.cpp
)

target_include_directories(udp_launcher PRIVATE
    ${CMAKE_SOURCE_DIR}/src/LS/client
    ${CMAKE_SOURCE_DIR}/src/common
)

target_link_libraries(udp_launcher PRIVATE common)
