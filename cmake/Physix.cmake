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
        message(FATAL_ERROR "${target_name} requires at least one source file")
    endif()

    add_executable("${target_name}" ${ARG_SOURCES})
    target_link_libraries("${target_name}" PRIVATE physix SDL3::SDL3)
endfunction()

function(physix_add_all_executables)
    cmake_parse_arguments(ARG "" "" "EXCLUDE" ${ARGN})

    file(
        GLOB_RECURSE source_files
        CONFIGURE_DEPENDS
        "${PHYSIX_ROOT}/*.cpp"
    )

    # Ignore generated sources when the build directory is inside the project.
    list(
        FILTER source_files
        EXCLUDE REGEX "/(impl|build|cmake-build-[^/]*)/"
    )

    foreach(source_file IN LISTS source_files)
        get_filename_component(executable_name "${source_file}" NAME_WLE)
        list(FIND ARG_EXCLUDE "${executable_name}" excluded_index)

        if(excluded_index EQUAL -1)
            if(TARGET "${executable_name}")
                message(
                    FATAL_ERROR
                    "Cannot create executable '${executable_name}': "
                    "another .cpp file has the same filename"
                )
            endif()

            physix_add_executable(
                "${executable_name}"
                SOURCES "${source_file}"
            )

            file(
                RELATIVE_PATH source_relative_path
                "${PHYSIX_ROOT}"
                "${source_file}"
            )
            get_filename_component(
                source_relative_directory
                "${source_relative_path}"
                DIRECTORY
            )

            if(source_relative_directory)
                set_target_properties(
                    "${executable_name}"
                    PROPERTIES
                        RUNTIME_OUTPUT_DIRECTORY
                            "${CMAKE_BINARY_DIR}/${source_relative_directory}"
                )
            endif()
        endif()
    endforeach()
endfunction()
