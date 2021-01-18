cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_extras_opengl_examples_basic CXX)

set(CMAKE_CXX_STANDARD 11)
include(../../opengl.cmake.in)
add_executable(basic
        midiread.cpp
        ../../src/st7735_opengl.cpp)
target_link_libraries(basic ${teensy_x86_stubs_LIBS})
target_link_libraries(basic ${teensy_st7735_linux_stubs_LIBS})
target_link_libraries(basic ${LIBS})