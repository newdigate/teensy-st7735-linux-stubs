cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_stubs C CXX)
set(teensy_st7735_linux_stubs_VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

find_library(teensy_x86_stubs NAME teensy_x86_stubs)
include_directories(${teensy_x86_stubs_INCLUDE_DIR})

find_library(teensy_gfx NAME teensy_gfx)
include_directories(${teensy_gfx_INCLUDE_DIR})

set(SOURCE_FILES
		ST7735_t3.cpp
		st7735_t3_font_Arial.c
		st7735_t3_font_ComicSansMS.c
		)

set(HEADER_FILES
		ST7735_t3.h
		st7735_t3_font_Arial.h
		st7735_t3_font_ComicSansMS.h)

add_library(teensy_st7735_linux_stubs SHARED STATIC ${HEADER_FILES} ${SOURCE_FILES})
