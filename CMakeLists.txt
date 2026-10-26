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

physix_add_executable(v1
    SOURCES projectile-motion/v1.cpp
)

physix_add_executable(two_body_gpu
    SOURCES two-body-gpu/two_body_gpu.cpp
)

set_target_properties(system PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/solar-system"
)

set_target_properties(v1 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/projectile-motion"
)

set_target_properties(two_body_gpu PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/two-body-gpu"
)

# The GPU example loads its shaders beside its executable.
add_custom_target(two_body_gpu_shaders
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_FILE_DIR:two_body_gpu>/shaders"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/two-body-gpu/shaders/MSL"
        "$<TARGET_FILE_DIR:two_body_gpu>/shaders/MSL"
    VERBATIM
)

foreach(shader_format IN ITEMS SPIRV DXIL)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/two-body-gpu/shaders/${shader_format}")
        add_custom_command(TARGET two_body_gpu_shaders POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/two-body-gpu/shaders/${shader_format}"
                "$<TARGET_FILE_DIR:two_body_gpu>/shaders/${shader_format}"
            VERBATIM
        )
    endif()
endforeach()

add_dependencies(two_body_gpu two_body_gpu_shaders)

