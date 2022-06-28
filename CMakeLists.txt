#
cmake_minimum_required(VERSION 3.6.0)

#
project(thunder VERSION 1.0.0 LANGUAGES C)

#
add_library(thunder
    src/source.c
)

#
target_include_directories(thunder
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
