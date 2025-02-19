cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_extras_opengl_examples_button C CXX)
set(CMAKE_CXX_STANDARD 17)

# comment out the line below to reference source, rather than referencing installed libraries (default)

include_directories(../../src)

set(APP_SOURCES graphicstest.cpp)

add_executable(button  ${APP_SOURCES})

target_link_libraries(button teensy_x86_stubs)
target_link_libraries(button teensy_x86_bounce2_stubs)
target_link_libraries(button teensy_st7735_linux_stubs)
target_link_libraries(button teensy_st7735_linux_opengl)
target_link_libraries(button teensy_x86_encoder_stubs)
target_link_libraries(button teensy_gfx)

target_link_libraries(button ${LIBS})

