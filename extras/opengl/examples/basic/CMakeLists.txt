cmake_minimum_required(VERSION 3.10)
project(teensy_st7735_linux_extras_opengl_examples_basic C CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(teensy_x86_stubs)
include_directories(${teensy_x86_stubs_INCLUDE_DIR})

find_package(teensy_x86_encoder_stubs)
include_directories(${teensy_x86_encoder_stubs_INCLUDE_DIR})

find_package(teensy_x86_bounce2_stubs)
include_directories(${teensy_x86_bounce2_stubs_INCLUDE_DIR})

find_package(teensy_st7735_linux_stubs)
include_directories(${teensy_st7735_linux_stubs_INCLUDE_DIR})

include_directories(../../src)

add_definitions(-fms-extensions)
add_definitions(-DArduino=146)

add_executable(basic graphicstest.cpp)

target_link_libraries(basic ${teensy_x86_stubs_LIBS})
target_link_libraries(basic ${teensy_x86_sd_stubs_LIBS})
target_link_libraries(basic ${teensy_x86_bounce2_stubs_LIBS})
target_link_libraries(basic ${teensy_x86_encoder_stubs_LIBS})
target_link_libraries(basic ${teensy_audio_x86_stubs_LIBS})
target_link_libraries(basic ${teensy_audio_x86_stubs_soundio_LIBS})
target_link_libraries(basic -L/usr/local/lib/ rtmidi)
target_link_libraries(basic ${teensy_st7735_linux_stubs_LIBS})
target_link_libraries(basic ${teensy_st7735_linux_extras_opengl_LIBS})

include(../../opengl.cmake.in)
target_link_libraries(basic ${LIBS})

target_link_libraries(basic "-L/usr/local/lib -lsoundio")
if  (APPLE)
        target_link_libraries(basic
                "-framework CoreServices"
                "-framework CoreAudio"
                "-framework Foundation"
                "-framework AudioUnit")
endif()