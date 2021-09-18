cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_extras_opengl_examples_basic C CXX)
set(CMAKE_CXX_STANDARD 11)

# comment out the line below to reference source, rather than referencing installed libraries (default)

include(../../common.cmake.in)
include(../../opengl.cmake.in)

set(APP_SOURCES graphicstest.cpp)

add_executable(basic  ${APP_SOURCES})

target_link_libraries(basic ${teensy_x86_stubs_LIBS})
target_link_libraries(basic ${teensy_st7735_linux_stubs_LIBS})
target_link_libraries(basic teensy_st7735_linux_opengl)
target_link_libraries(basic ${LIBS})

