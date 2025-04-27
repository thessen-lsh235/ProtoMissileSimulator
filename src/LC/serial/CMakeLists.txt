# LC 시리얼 통신 모듈
add_library(lc_serial STATIC
    lc_serial.cpp
)

# include 경로 설정
target_include_directories(lc_serial PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../../common
)
