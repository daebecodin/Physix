cmake_minimum_required(VERSION 3.27)
project(physix LANGUAGES CXX)

include("${CMAKE_CURRENT_LIST_DIR}/cmake/Physix.cmake")

add_subdirectory("/Users/dmac/SDL" "${CMAKE_BINARY_DIR}/SDL" EXCLUDE_FROM_ALL)

physix_add_all_executables()

# The GPU example loads its shaders beside its executable.
add_custom_target(two_body_gpu_shaders
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_FILE_DIR:two_body_gpu>/shaders"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/two-body-gpu/shaders/MSL"
        "$<TARGET_FILE_DIR:two_body_gpu>/shaders/MSL"
)
foreach(shader_format IN ITEMS SPIRV DXIL)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/two-body-gpu/shaders/${shader_format}")
        add_custom_command(TARGET two_body_gpu_shaders POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/two-body-gpu/shaders/${shader_format}"
                "$<TARGET_FILE_DIR:two_body_gpu>/shaders/${shader_format}"
        )
    endif()
endforeach()
add_dependencies(two_body_gpu two_body_gpu_shaders)

