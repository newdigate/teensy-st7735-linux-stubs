cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_opengl CXX)
set(teensy_st7735_linux_opengl_VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)

find_package(teensy_x86_stubs)
include_directories(${teensy_x86_stubs_INCLUDE_DIR})

find_package(teensy_x86_sd_stubs)
include_directories(${teensy_x86_sd_stubs_INCLUDE_DIR})

find_package(teensy_st7735_linux_stubs)
include_directories(${teensy_st7735_linux_stubs_INCLUDE_DIR})

include(../opengl.cmake.in)

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/teensy_st7735_linux_opengl/)
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib/teensy_st7735_linux_opengl )

set(SOURCE_FILES
        st7735_opengl.cpp)

set(HEADER_FILES
        st7735_opengl.h
        st7735_opengl_main.h)

add_library(teensy_st7735_linux_opengl SHARED STATIC ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(teensy_st7735_linux_opengl PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

include(CMakePackageConfigHelpers)
configure_package_config_file(../cmake/teensy_st7735_linux_opengl.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/teensy_st7735_linux_openglConfig.cmake
        INSTALL_DESTINATION ${LIB_INSTALL_DIR}/teensy_st7735_linux_opengl/cmake
        PATH_VARS)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/teensy_st7735_linux_openglConfigVersion.cmake
        VERSION ${teensy_st7735_linux_opengl_VERSION}
        COMPATIBILITY SameMajorVersion )

install(TARGETS teensy_st7735_linux_opengl DESTINATION
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
        PUBLIC_HEADER DESTINATION "include/teensy_st7735_linux_opengl"
        )

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/teensy_st7735_linux_openglConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/teensy_st7735_linux_openglConfigVersion.cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/uninstall.cmake
        DESTINATION "lib/cmake/teensy_st7735_linux_opengl" )

##
# copied from: https://gist.github.com/royvandam/3033428
# Add uninstall target
# Requirements: Copy the uninstall.cmake file to the appropriate CMAKE_MODULE_PATH.
add_custom_target(uninstall
        "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/uninstall.cmake")