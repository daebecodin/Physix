cmake_minimum_required(VERSION 3.20)
project(physix LANGUAGES CXX)

include("${CMAKE_CURRENT_LIST_DIR}/cmake/Physix.cmake")

physix_add_all_executables()
