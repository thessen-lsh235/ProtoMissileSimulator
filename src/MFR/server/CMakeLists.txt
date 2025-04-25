add_executable(MFR
    MFR.cpp
)

target_include_directories(MFR PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MFR PRIVATE common)