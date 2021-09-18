cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_extras_opengl_examples_scrolltest C CXX)
set(CMAKE_CXX_STANDARD 11)

# comment out the line below to reference source, rather than referencing installed libraries (default)
set(USE_INSTALLED_ST7735 1)

include(../../common.cmake.in)
include(../../opengl.cmake.in)

set(APP_SOURCES scrolltest.cpp)

add_executable(scrolltest  ${APP_SOURCES})

target_link_libraries(scrolltest ${teensy_x86_stubs_LIBS})
target_link_libraries(scrolltest ${teensy_st7735_linux_stubs_LIBS})
target_link_libraries(scrolltest ${teensy_st7735_linux_extras_opengl_LIBS})
target_link_libraries(scrolltest ${LIBS})