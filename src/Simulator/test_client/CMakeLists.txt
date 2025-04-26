set(CMAKE_CXX_STANDARD 17)

# common 헤더 경로 추가
include_directories(../)
include_directories(${CMAKE_SOURCE_DIR}/../common)

# target_command_client 실행 파일 생성 및 common 라이브러리 링크
add_executable(target_command_client target_command_client.cpp)
target_link_libraries(target_command_client PRIVATE common)

# missile_command_client 실행 파일 생성 및 common 라이브러리 링크
add_executable(missile_command_client missile_command_client.cpp)
target_link_libraries(missile_command_client PRIVATE common)
