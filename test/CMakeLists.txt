include(FetchContent)

FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.11
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(doctest)


file(GLOB TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test_*.cc")
message(${TEST_FILES})
add_executable(unittest ${TEST_FILES})
target_link_libraries(unittest doctest_with_main cjson)


