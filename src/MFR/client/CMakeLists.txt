add_executable(SIM
    SIM.cpp
    defineAlgorithm.cpp
    loaderINI.cpp
)

target_include_directories(SIM PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SIM PRIVATE common)