include_guard(GLOBAL)

set(
    PHYSIX_CXX_STANDARD
    20
    CACHE STRING
    "C++ language standard used by Physix targets"
)

set(CMAKE_CXX_STANDARD "${PHYSIX_CXX_STANDARD}")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

get_filename_component(
    PHYSIX_ROOT
    "${CMAKE_CURRENT_LIST_DIR}/.."
    ABSOLUTE
)

if(NOT TARGET physix_pch)
    add_library(physix_pch INTERFACE)

    target_compile_options(
        physix_pch
        INTERFACE
            "$<$<COMPILE_LANG_AND_ID:CXX,AppleClang,Clang,GNU>:-Wall;-Wextra;-Wpedantic>"
    )

    target_include_directories(
        physix_pch
        INTERFACE "${PHYSIX_ROOT}/include"
    )

    target_precompile_headers(
        physix_pch
        INTERFACE "${PHYSIX_ROOT}/include/pch.h"
    )
endif()

if(NOT TARGET physix)
    add_library(physix STATIC
        "${PHYSIX_ROOT}/impl/physix.cpp"
        "${PHYSIX_ROOT}/impl/physix_calculations.cpp"
        "${PHYSIX_ROOT}/impl/physix_input.cpp"
    )

    target_link_libraries(physix PUBLIC physix_pch)
endif()

function(physix_add_executable target_name)
    cmake_parse_arguments(ARG "" "" "SOURCES" ${ARGN})

    if(NOT ARG_SOURCES)
        message(
            FATAL_ERROR
            "${target_name} requires at least one source file"
        )
    endif()

    add_executable("${target_name}" ${ARG_SOURCES})

    target_link_libraries(
        "${target_name}"
        PRIVATE physix SDL3::SDL3
    )
endfunction()

