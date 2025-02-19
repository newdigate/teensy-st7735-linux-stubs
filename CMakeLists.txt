cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux CXX)

set(CMAKE_CXX_STANDARD 17)

include(cmake_declare_and_fetch.cmake.in)
add_definitions(-DBUILD_FOR_LINUX)
DeclareAndFetch(teensy_x86_stubs https://github.com/newdigate/teensy-x86-stubs.git main src)
DeclareAndFetch(teensy_gfx https://github.com/newdigate/teensy-gfx.git main src)

add_subdirectory(src)
add_subdirectory(extras/opengl)
