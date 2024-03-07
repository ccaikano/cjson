cmake_minimum_required(VERSION 3.27)
project(cjson)

set(CMAKE_CXX_STANDARD 17)

add_library(cjson SHARED
        src/cjson/json_object.cc
        src/cjson/json_object.hpp
        src/cjson/value_t.hpp)

target_include_directories(cjson INTERFACE ${PROJECT_SOURCE_DIR}/src/cjson)


option(CJSON_UNITTEST "unit test" OFF)

if (CJSON_UNITTEST)
    enable_testing()
    add_subdirectory(test)
endif ()