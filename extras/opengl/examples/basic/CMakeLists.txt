cmake_minimum_required(VERSION 3.10)
project(teensy_st7735_linux_extras_opengl_examples_basic C CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(../../src)

add_definitions(-fms-extensions)
add_definitions(-DArduino=146)

add_executable(basic graphicstest.cpp)

target_link_libraries(basic teensy_x86_stubs)
target_link_libraries(basic teensy_x86_sd_stubs)
target_link_libraries(basic teensy_x86_bounce2_stubs)
target_link_libraries(basic teensy_x86_encoder_stubs)
target_link_libraries(basic teensy_gfx)
target_link_libraries(basic teensy_st7735_linux_stubs)
target_link_libraries(basic teensy_st7735_linux_opengl)

include(../../opengl.cmake.in)
target_link_libraries(basic ${LIBS})

if  (APPLE)
        target_link_libraries(basic
                "-framework CoreServices"
                "-framework CoreAudio"
                "-framework Foundation"
                "-framework AudioUnit")
endif()