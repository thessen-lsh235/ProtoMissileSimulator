add_executable(udp_launcher udp_launcher.cpp ini.c)

# common 헤더 사용
target_include_directories(udp_launcher PRIVATE ${CMAKE_SOURCE_DIR}/../common)

# 공용 common 라이브러리 링크
target_link_libraries(udp_launcher PRIVATE common)
