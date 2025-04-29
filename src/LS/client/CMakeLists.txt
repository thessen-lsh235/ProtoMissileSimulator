add_library(udp_launcher STATIC udp_launcher.cpp)

target_include_directories(udp_launcher PRIVATE
    ../common
    ../common/ini_parser
)

target_link_libraries(udp_launcher PRIVATE common)
