cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_extras_opengl_examples_spitftbitmap C CXX)
set(CMAKE_CXX_STANDARD 11)

include(../../common.cmake.in)
include(../../opengl.cmake.in)

set(APP_SOURCES spitftbitmap.cpp)

add_executable(spitftbitmap  ${APP_SOURCES})

target_link_libraries(spitftbitmap ${teensy_x86_stubs_LIBS})
target_link_libraries(spitftbitmap ${teensy_st7735_linux_stubs_LIBS})
target_link_libraries(spitftbitmap teensy_st7735_linux_opengl)
target_link_libraries(spitftbitmap ${teensy_x86_sd_stubs_LIBS} ${LIBS})