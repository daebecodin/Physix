cmake_minimum_required(VERSION 3.27)
project(physix LANGUAGES CXX)

include("${CMAKE_CURRENT_LIST_DIR}/cmake/Physix.cmake")

add_subdirectory(
    "/Users/dmac/SDL"
    "${CMAKE_BINARY_DIR}/SDL"
    EXCLUDE_FROM_ALL
)

physix_add_executable(system
    SOURCES solar-system/system.cpp
)
target_link_libraries(system PRIVATE SDL3::SDL3)

physix_add_executable(v1
    SOURCES projectile-motion/v1.cpp
)

set_target_properties(system PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/solar-system"
)

set_target_properties(v1 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/projectile-motion"
)

