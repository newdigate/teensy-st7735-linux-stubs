cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_extras_opengl_examples_spitftbitmap C CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(../../src)
include_directories(../../../../src)

set(APP_SOURCES spitftbitmap.cpp)

add_executable(spitftbitmap  ${APP_SOURCES})

target_link_libraries(spitftbitmap teensy_x86_stubs)
target_link_libraries(spitftbitmap teensy_x86_bounce2_stubs)
target_link_libraries(spitftbitmap teensy_st7735_linux_stubs)
target_link_libraries(spitftbitmap teensy_st7735_linux_opengl)
target_link_libraries(spitftbitmap teensy_x86_sd_stubs ${LIBS})
target_link_libraries(spitftbitmap teensy_gfx)
