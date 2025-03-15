cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_opengl CXX)
set(teensy_st7735_linux_opengl_VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)
include(../opengl.cmake.in)
set(SOURCE_FILES
        st7735_opengl.cpp
        st7735_opengl_main.cpp)

add_library(teensy_st7735_linux_opengl STATIC ${SOURCE_FILES})
