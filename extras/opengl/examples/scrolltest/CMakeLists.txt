cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_extras_opengl_examples_scrolltest C CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(../../src)
include(../../opengl.cmake.in)

set(APP_SOURCES scrolltest.cpp)

add_executable(scrolltest  ${APP_SOURCES})

target_link_libraries(scrolltest teensy_x86_stubs)
target_link_libraries(scrolltest teensy_x86_bounce2_stubs)
target_link_libraries(scrolltest teensy_st7735_linux_stubs)
target_link_libraries(scrolltest teensy_st7735_linux_opengl)
target_link_libraries(scrolltest ${LIBS})
target_link_libraries(scrolltest teensy_gfx)
