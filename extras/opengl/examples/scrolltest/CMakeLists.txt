cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_extras_opengl_examples_scrolltest C CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(teensy_x86_stubs)
include_directories(${teensy_x86_stubs_INCLUDE_DIR})

find_package(teensy_gfx)
include_directories(${teensy_gfx_INCLUDE_DIR})

find_package(teensy_x86_sd_stubs)
include_directories(${teensy_x86_sd_stubs_INCLUDE_DIR})

find_package(teensy_x86_encoder_stubs)
include_directories(${teensy_x86_encoder_stubs_INCLUDE_DIR})

find_package(teensy_x86_bounce2_stubs)
include_directories(${teensy_x86_bounce2_stubs_INCLUDE_DIR})

find_package(teensy_st7735_linux_stubs)
include_directories(${teensy_st7735_linux_stubs_INCLUDE_DIR})

include_directories(../../src)
include(../../opengl.cmake.in)

set(APP_SOURCES scrolltest.cpp)

add_executable(scrolltest  ${APP_SOURCES})

target_link_libraries(scrolltest ${teensy_x86_stubs_LIBS})
target_link_libraries(scrolltest ${teensy_x86_bounce2_stubs_LIBS})
target_link_libraries(scrolltest ${teensy_st7735_linux_stubs_LIBS})
target_link_libraries(scrolltest ${teensy_st7735_linux_extras_opengl_LIBS})
target_link_libraries(scrolltest ${LIBS})
target_link_libraries(scrolltest ${teensy_gfx_LIBS})
