//
// Created by CC on 2024/3/7.
//

#include <doctest/doctest.h>
#include <type_traits>
#include <iostream>
#include <cjson/number.h>
#include <cjson/json_object.h>

TEST_SUITE_BEGIN("unit");

using namespace cc::cjson;
number<> add(number<> a, number<> b)  {

  return number(1);
}
TEST_CASE("test")
{
  std::cout << is_number<int64_t >::value << std::endl;       // 1 (true)
  std::cout << is_number<double>::value << std::endl;    // 1 (true)
  std::cout << is_number<char>::value << std::endl;      // 0 (false)
  std::cout << is_number<bool>::value << std::endl;      // 0 (false)
  std::cout << is_number<std::string>::value << std::endl;// 0 (false)


  CHECK(add(1, 2) == 1);
}
TEST_CASE("jsonObject")
{
   JsonObject j1(true);
//  CHECK(j1.value<std::string_view>() == "");
//  CHECK(j1.value<bool>() == 0);

}

TEST_SUITE_END;