//
// Created by CC on 2024/3/7.
//

#include <doctest/doctest.h>
#include <type_traits>
#include <iostream>

TEST_SUITE_BEGIN("unit");

struct number {
  union {
    double d_;
    int i_;
  };
  template<typename T, typename std::enable_if_t<std::is_integral_v<std::decay_t<T>>, bool> = true>
  explicit number(T i) : i_(i) {
    std::cout << "int" << std::endl;
  }

  template<class T, typename std::enable_if_t<std::is_floating_point_v<std::decay_t<T>>, bool> = true>
  explicit number(T d) : d_(d) {
    std::cout << "double" << std::endl;
  }
};
TEST_CASE("test")
{
  const int a = 10;
  number i{a};
  number d{120.6f};
}

TEST_SUITE_END;