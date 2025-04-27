add_library(lc_logger STATIC
    lc_logger.cpp
)
target_include_directories(lc_logger PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../../common
)