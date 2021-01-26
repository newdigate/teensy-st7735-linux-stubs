cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_extras_opengl_examples_spitftbitmap C CXX)
set(CMAKE_CXX_STANDARD 11)

# comment out the line below to reference source, rather than referencing installed libraries (default)
set(USE_INSTALLED_ST7735 1)

if (NOT DEFINED USE_INSTALLED_ST7735)
    if (NOT DEFINED teensy_x86_stubs_root)
        set(teensy_st7735_stubs_root ${CMAKE_SOURCE_DIR}/../../../..)
    endif()
endif()

include(../../common.cmake.in)
include(../../opengl.cmake.in)

set(APP_SOURCES spitftbitmap.cpp ${ST7735_OPENGL_SOURCES})

if (NOT DEFINED USE_INSTALLED_ST7735)
    set(APP_SOURCES ${APP_SOURCES} ${ST7735_SOURCES})
endif()

add_executable(spitftbitmap  ${APP_SOURCES})

target_link_libraries(spitftbitmap ${teensy_x86_stubs_LIBS})
if (DEFINED USE_INSTALLED_ST7735)
    target_link_libraries(spitftbitmap ${teensy_st7735_linux_stubs_LIBS})
endif()
target_link_libraries(spitftbitmap ${LIBS})