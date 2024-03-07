//
// Created by CC on 2024/3/7.
//

#pragma once

#include <map> // map
#include <string> // string
#include <vector> // vector
#include <type_traits>
#include "value_t.hpp"

namespace cc {
namespace cjson {

/**
 * @brief json对象类
 */
class JsonObject {
 public:

  struct number;
  /// 字符串类型
  using string_t = std::string;
  /// 对象类型
  using object_t = std::map<string_t, JsonObject>;
  /// 数组类型
  using array_t = std::vector<JsonObject>;
  /// 布尔类型
  using boolean_t = bool;
  /// 数字类型
  using int_t = int64_t;
  using double_t = double;
  using number_t = number;



  struct number {
    union {
      double_t m_double;
      int_t m_int;
    };

    template<typename T, typename std::enable_if_t<std::is_integral_v<std::decay_t<T>>, bool> = true>
    explicit number(T i) : m_int(i) {

    }

    template<class T, typename std::enable_if_t<std::is_floating_point_v<std::decay_t<T>>, bool> = true>
    explicit number(T d) : m_double(d) {
    }

  };



  union json_value {
    object_t *object;
    array_t *array;
    string_t *string;
    boolean_t boolean;
    number_t number;

    json_value() = default;

    json_value(boolean_t v) noexcept: boolean(v) {}
    json_value(number_t v) noexcept: number(v) {}



  };

  template<typename T,typename ... Args>
  static T* create(Args && ... args) {
    // todo 内存分配器
    return new T(std::forward<Args>(args)...);
  }


 private:
  struct data {
    value_t m_type{value_t::null};

  };




  /// json数据
  data m_data{};
};

} // cjson
} // cc
