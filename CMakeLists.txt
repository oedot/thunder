#----------------------------------------------------------------
#
#----------------------------------------------------------------

#
cmake_minimum_required(VERSION 3.6.0)

#
project(thunder VERSION 1.0.0 LANGUAGES C CXX)

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

#----------------------------------------------------------------
#
#----------------------------------------------------------------

#
option(THUNDER_BUILD_TESTS "" ON)

#
if(THUNDER_BUILD_TESTS)
    #
    enable_testing()
    #
    include(GoogleTest)
    #
    add_subdirectory(tests)

endif()
