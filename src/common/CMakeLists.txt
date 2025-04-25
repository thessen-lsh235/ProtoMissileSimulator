# 헤더 전용 라이브러리 생성
add_library(common STATIC
    ini_parser/ini_parser.cpp
)

# include 디렉토리 설정
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(test)