cmake_minimum_required(VERSION 3.20)

project(cjson VERSION 1.0.0 LANGUAGES CXX)

# 输出编译时的详细信息
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions($<$<CONFIG:Debug>:CC_DEBUG>)

# # 检测编译器类型并设置相应的编译选项
# if(MSVC)
# # 使用 MSVC 编译器
# add_compile_options(/W4 /permissive-)
# elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
# # 使用 GCC 编译器
# add_compile_options(-Wall -Wextra -Wpedantic)
#
# elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
# # 使用 Clang 编译器
# add_compile_options(-Wall -Wextra -Wpedantic)
#
# endif()
# file(GLOB_RECURSE headers include/cjson/*.h include/cjson/*.hpp)
add_library(${PROJECT_NAME} INTERFACE)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} INTERFACE IS_WIN)
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} INTERFACE IS_MAC)

    if(IOS)
        target_compile_definitions(${PROJECT_NAME} INTERFACE IS_IOS)
    endif()
elseif(UNIX)
    if(CMAKE_SYSTEM_NAME MATCHES "Linux")
        target_compile_definitions(${PROJECT_NAME} INTERFACE IS_LINUX)
    elseif(CMAKE_SYSTEM_NAME MATCHES "Android")
        target_compile_definitions(${PROJECT_NAME} INTERFACE IS_ANDROID)
    endif()
endif()

# 定义操作系统家族
if(APPLE)
    target_compile_definitions(${PROJECT_NAME} INTERFACE IS_APPLE)
endif()

if(UNIX AND NOT WIN32)
    target_compile_definitions(${PROJECT_NAME} INTERFACE IS_POSIX)
endif()

# add_compile_definitions(JSON_NOEXCEPTION)
# set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# 是否开启单元测试
option(CJSON_UNITTEST "unit test" ON)
message(STATUS "CJSON_UNITTEST: ${CJSON_UNITTEST}")

# if (CJSON_UNITTEST)
enable_testing()
add_subdirectory(test)

# endif ()
