//
// Created by CC on 2024/3/7.
//

#pragma once

#include <map> // map
#include <string> // string
#include <vector> // vector
#include <variant>

#include <cjson/value_t.h>
#include <cjson/number.h>
#include <cjson/macro_scope.h>
#include <cjson/type_traits.h>

namespace cc::cjson {
/**
 * @brief json对象类
 */
class JsonObject {
 public:
  /// 字符类型
  using char_t = char;
  /// 字符串类型
  using string_t = std::basic_string<char_t>;
  /// 对象类型
  using object_t = std::map<string_t, JsonObject>;
  /// 数组类型
  using array_t = std::vector<JsonObject>;
  /// 布尔类型
  using boolean_t = bool;

  using integer_t = std::int64_t;

  using float_t = double;

  /// 数字类型
  using number_t = number<integer_t , float_t>;
  /// 值类型
  using data_t = std::variant<number_t, string_t, boolean_t, object_t, array_t>;

  JsonObject() : m_data(nullptr), m_type(value_t::Null) {}

  JsonObject(const JsonObject &other) = delete;

  JsonObject &operator=(const JsonObject &other) = delete;

  JsonObject(JsonObject &&other) noexcept = default;

  JsonObject &operator=(JsonObject &&) noexcept = default;


  template<typename T, typename std::enable_if_t<is_number<T>::value, int> = 0>
  explicit JsonObject(T value):m_data(number_t(value)), m_type(value_t::Number) {}

  explicit JsonObject(boolean_t value) : m_data(value), m_type(value_t::Boolean) {}

  explicit JsonObject(const char_t *value) : m_data(string_t(value)), m_type(value_t::String) {}

  explicit JsonObject(string_t value) : m_data(std::move(value)), m_type(value_t::String) {}

  explicit JsonObject(object_t value) : m_data(std::move(value)), m_type(value_t::Object) {}

  explicit JsonObject(array_t value) : m_data(std::move(value)), m_type(value_t::Array) {}

  [[nodiscard]] value_t type() const { return m_type; }

  [[nodiscard]] bool isNull() const { return m_type == value_t::Null; }

  [[nodiscard]] bool isNumber() const { return m_type == value_t::Number; }

  [[nodiscard]] bool isInteger() const { return m_type == value_t::Number && std::get_if<number_t>(&m_data)->isInteger(); }

  [[nodiscard]] bool isFloat() const { return m_type == value_t::Number && std::get_if<number_t>(&m_data)->isFloat(); }

  [[nodiscard]] bool isString() const { return m_type == value_t::String; }

  [[nodiscard]] bool isBoolean() const { return m_type == value_t::Boolean; }

  [[nodiscard]] bool isObject() const { return m_type == value_t::Object; }

  [[nodiscard]] bool isArray() const { return m_type == value_t::Array; }

  [[nodiscard]] bool contains(const string_t &key) const {
    if (CC_LIKELY(isObject())) {
      return asObject().count(key) > 0;
    }
    return false;
  }



  [[nodiscard]] integer_t asInteger() const {
    if (CC_LIKELY(isNumber())) {
      return std::get<number_t>(m_data).asInteger();
    }
    THROW_ERROR("Not a number")
  }

  [[nodiscard]] float_t asFloat() const {
    if (CC_LIKELY(isNumber())) {
      return std::get<number_t>(m_data).asFloat();
    }
    THROW_ERROR("Not a number")
  }

  [[nodiscard]] boolean_t asBoolean() const {
    if (CC_LIKELY(isBoolean())) {
      return std::get<boolean_t>(m_data);
    }
    THROW_ERROR("Not a boolean")
  }


  [[nodiscard]] const string_t& asString() const {
    if (CC_LIKELY(isString())) {
      return std::get<string_t>(m_data);
    }
    THROW_ERROR("Not a string")
  }

  [[nodiscard]] const object_t& asObject() const {
    if (CC_LIKELY(isObject())) {
      return std::get<object_t>(m_data);
    }
    THROW_ERROR("Not an object")
  }

  [[nodiscard]] const array_t& asArray() const {
    if (CC_LIKELY(isArray())) {
      return std::get<array_t>(m_data);
    }
    THROW_ERROR("Not an array")
  }


 private:
  data_t m_data;
  value_t m_type;
};
} // cc::cjson


#include <cjson/macro_unscope.h>