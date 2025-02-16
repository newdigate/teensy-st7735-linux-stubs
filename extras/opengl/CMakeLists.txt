cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_extras_opengl CXX)
include(../../cmake_declare_and_fetch.cmake.in)
set(CMAKE_CXX_STANDARD 17)

DeclareAndFetch(teensy_x86_stubs https://github.com/newdigate/teensy-x86-stubs.git noinstall src)
DeclareAndFetch(teensy_x86_sd_stubs https://github.com/newdigate/teensy-x86-sd-stubs.git noinstall src)
DeclareAndFetch(teensy_gfx https://github.com/newdigate/teensy-gfx.git noinstall src)
DeclareAndFetch(teensy_x86_encoder_stubs https://github.com/newdigate/teensy-x86-encoder-stubs.git noinstall src)
DeclareAndFetch(teensy_x86_bounce2_stubs https://github.com/newdigate/teensy-x86-bounce2-stubs.git noinstall src)

include(opengl.cmake.in)

add_subdirectory(src)
add_subdirectory(examples)