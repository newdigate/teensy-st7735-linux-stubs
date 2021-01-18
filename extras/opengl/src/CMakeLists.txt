cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_extras_opengl CXX)
set(CMAKE_CXX_STANDARD 11)
include(../opengl.cmake.in)

add_library(teensy_st7735_linux_opengl
        st7735_opengl.cpp
        st7735_opengl.h)