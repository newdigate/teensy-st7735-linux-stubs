cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux CXX)

set(CMAKE_CXX_STANDARD 17)

include(Dependencies.CMake.in)

add_subdirectory(src)
