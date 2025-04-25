# C++17 사용
set(CMAKE_CXX_STANDARD 17)

# 테스트 실행 파일 생성
add_executable(test_main main.cpp)

# common 라이브러리 링크
target_link_libraries(test_main PRIVATE common)

# common 디렉토리를 포함 경로로 추가
target_include_directories(test_main PRIVATE ${CMAKE_SOURCE_DIR}/src/common)