include(FetchContent)

#FetchContent_Declare(
#        doctest
#        GIT_REPOSITORY https://github.com/doctest/doctest.git
#        GIT_TAG v2.4.11
#        GIT_SHALLOW TRUE
#)
#FetchContent_MakeAvailable(doctest)


#file(GLOB_RECURSE TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test_*.cc")
#foreach (file  ${TEST_FILES})
#    # 获取文件名（不包括路径和扩展名）
#    get_filename_component(filename ${file} NAME_WE)
#    add_executable(${filename} ${file}
#            test_string.cc)
#
#    target_link_libraries(${filename} doctest_with_main cjson)
#endforeach()




add_executable(test_string test_string.cc)

target_link_libraries(test_string cjson)