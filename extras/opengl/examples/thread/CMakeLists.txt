cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_extras_opengl_examples_basic C CXX)
set(CMAKE_CXX_STANDARD 11)

include_directories(../../src)

set(APP_SOURCES thread.cpp)

add_executable(thread ${APP_SOURCES})

target_link_libraries(thread teensy_x86_stubs)
target_link_libraries(thread teensy_x86_bounce2_stubs)
target_link_libraries(thread teensy_st7735_linux_stubs)
target_link_libraries(thread teensy_st7735_linux_opengl)
target_link_libraries(thread ${LIBS})
target_link_libraries(thread teensy_gfx)


