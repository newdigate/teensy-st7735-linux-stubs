cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_extras_opengl_examples_button C CXX)
set(CMAKE_CXX_STANDARD 17)

# comment out the line below to reference source, rather than referencing installed libraries (default)

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

set(APP_SOURCES graphicstest.cpp)

add_executable(button  ${APP_SOURCES})

target_link_libraries(button ${teensy_x86_stubs_LIBS})
target_link_libraries(button ${teensy_x86_bounce2_stubs_LIBS})
target_link_libraries(button ${teensy_st7735_linux_stubs_LIBS})
target_link_libraries(button ${teensy_st7735_linux_extras_opengl_LIBS})
target_link_libraries(button ${teensy_x86_encoder_stubs_LIBS})
target_link_libraries(button ${teensy_gfx_LIBS})

target_link_libraries(button ${LIBS})

