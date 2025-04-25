cmake_minimum_required(VERSION 3.10)
project(ProtoMissileSimulator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)
