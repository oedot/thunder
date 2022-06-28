#----------------------------------------------------------------
#
#----------------------------------------------------------------

#
add_subdirectory(${PROJECT_SOURCE_DIR}/extern/googletest extern/googletest)

# create an executable in which the tests will be stored
add_executable(thunder_tests
    src/source.cc
)

# link the Google Test infrastucture, mocking library and a default main function to
# the test executable.
target_link_libraries(thunder_tests
    PRIVATE
        gtest
        gtest_main
        gmock
        thunder
)

#
gtest_discover_tests(thunder_tests
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
