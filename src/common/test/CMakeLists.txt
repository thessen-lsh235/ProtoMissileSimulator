# C++17 사용
set(CMAKE_CXX_STANDARD 17)

# 테스트 실행 파일 생성
add_executable(test_main main.cpp)
add_executable(server server.cpp)
add_executable(client client.cpp)
add_executable(ini_parser_test ini_parser_test.cpp)

# common 라이브러리 링크
target_link_libraries(test_main PRIVATE common)
target_link_libraries(server PRIVATE common)
target_link_libraries(client PRIVATE common)
target_link_libraries(ini_parser_test PRIVATE common)

# common 디렉토리를 포함 경로로 추가
target_include_directories(test_main PRIVATE ${CMAKE_SOURCE_DIR}/src/common)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/common)
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/src/common)
target_include_directories(ini_parser_test PRIVATE ${CMAKE_SOURCE_DIR}/src/common)