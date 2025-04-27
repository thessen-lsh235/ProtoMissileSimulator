# LC 클라이언트 모듈
add_library(lc_client STATIC
    lc_client.cpp
)

target_include_directories(lc_client PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../../common
)