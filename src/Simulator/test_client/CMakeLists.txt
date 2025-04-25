set(CMAKE_CXX_STANDARD 17)

include_directories(../)

add_executable(target_command_client target_command_client.cpp)
add_executable(missile_command_client missile_command_client.cpp)
