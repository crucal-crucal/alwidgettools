cmake_minimum_required(VERSION 3.16)
project(alwidgettools VERSION 0.1 LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/cmake/alsetup.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/alconfigmacros.cmake)

uv_initialize_build_flags()
uv_setup_Qt()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(example)
