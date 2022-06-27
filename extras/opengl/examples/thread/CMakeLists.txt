cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_extras_opengl_examples_basic C CXX)
set(CMAKE_CXX_STANDARD 11)

include(../../common.cmake.in)
include(../../opengl.cmake.in)

set(APP_SOURCES thread.cpp)

add_executable(thread ${APP_SOURCES})

target_link_libraries(thread ${teensy_x86_stubs_LIBS})
target_link_libraries(thread ${teensy_x86_bounce2_stubs_LIBS})
target_link_libraries(thread ${teensy_st7735_linux_stubs_LIBS})
target_link_libraries(thread ${teensy_st7735_linux_extras_opengl_LIBS})
target_link_libraries(thread ${LIBS})


