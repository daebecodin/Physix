cmake_minimum_required(VERSION 3.27)
project(physix LANGUAGES CXX)

include("${CMAKE_CURRENT_LIST_DIR}/cmake/Physix.cmake")

set(PHYSIX_SDL_SOURCE_DIR "" CACHE PATH "Optional path to an SDL3 source checkout")
if(PHYSIX_SDL_SOURCE_DIR)
    add_subdirectory("${PHYSIX_SDL_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/SDL" EXCLUDE_FROM_ALL)
else()
    find_package(SDL3 CONFIG REQUIRED)
endif()

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

find_package(Matplot++ CONFIG REQUIRED)
physix_add_executable(orbit SOURCES demos/orbit/orbit.cpp)
target_link_libraries(orbit PRIVATE Matplot++::matplot)
