cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 17)

project(cjson LANGUAGES CXX)
# 检测编译器类型并设置相应的编译选项
if(MSVC)
  # 使用 MSVC 编译器
  add_compile_options(/W4 /permissive-)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  # 使用 GCC 编译器
  add_compile_options(-Wall -Wextra -Wpedantic)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # 使用 Clang 编译器
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

file(GLOB_RECURSE headers include/cjson/*.h include/cjson/*.hpp)
add_library(${PROJECT_NAME} SHARED ${headers})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include/)
#target_compile_definitions(${PROJECT_NAME}  JSON_EXCEPTION)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)


# 是否开启单元测试
option(CJSON_UNITTEST "unit test" OFF)

if (CJSON_UNITTEST)
  enable_testing()
  add_subdirectory(test)
endif ()
