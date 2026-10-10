cmake_minimum_required(VERSION 3.27)
project(physix LANGUAGES CXX)

include("${CMAKE_CURRENT_LIST_DIR}/cmake/Physix.cmake")

add_subdirectory("/Users/dmac/SDL" "${CMAKE_BINARY_DIR}/SDL" EXCLUDE_FROM_ALL)

physix_add_all_executables()

