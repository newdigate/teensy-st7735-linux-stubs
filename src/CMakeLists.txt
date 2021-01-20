cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_stubs C CXX)
set(teensy_st7735_linux_stubs_VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)

find_package(teensy_x86_stubs)
include_directories(${teensy_x86_stubs_INCLUDE_DIR})

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/teensy_st7735_linux_stubs/)
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib/teensy_st7735_linux_stubs )

set(SOURCE_FILES
		glcdfont.c
		ST7735_t3.cpp)

set(HEADER_FILES
		ILI9341_fonts.h
		ST7735_t3.h)

add_library(teensy_st7735_linux_stubs SHARED STATIC ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(teensy_st7735_linux_stubs PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

include(CMakePackageConfigHelpers)
configure_package_config_file(../cmake/teensy_st7735_linux_stubs.cmake.in
		${CMAKE_CURRENT_BINARY_DIR}/teensy_st7735_linux_stubsConfig.cmake
		INSTALL_DESTINATION ${LIB_INSTALL_DIR}/teensy_st7735_linux_stubs/cmake
		PATH_VARS)

write_basic_package_version_file(
		${CMAKE_CURRENT_BINARY_DIR}/teensy_st7735_linux_stubsConfigVersion.cmake
		VERSION ${teensy_st7735_linux_stubs_VERSION}
		COMPATIBILITY SameMajorVersion )

install(TARGETS teensy_st7735_linux_stubs DESTINATION
		LIBRARY DESTINATION ${LIB_INSTALL_DIR}
		ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
		PUBLIC_HEADER DESTINATION "include/teensy_st7735_linux_stubs"
		)

install(FILES
		${CMAKE_CURRENT_BINARY_DIR}/teensy_st7735_linux_stubsConfig.cmake
		${CMAKE_CURRENT_BINARY_DIR}/teensy_st7735_linux_stubsConfigVersion.cmake
		${CMAKE_CURRENT_SOURCE_DIR}/../cmake/uninstall.cmake
		DESTINATION "lib/cmake/teensy_st7735_linux_stubs" )

##
# copied from: https://gist.github.com/royvandam/3033428
# Add uninstall target
# Requirements: Copy the uninstall.cmake file to the appropriate CMAKE_MODULE_PATH.
add_custom_target(uninstall
		"${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake")