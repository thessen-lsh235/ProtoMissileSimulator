# LC 서버 모듈
add_library(lc_server STATIC
    lc_server.cpp
)
target_include_directories(lc_server PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../../common
)