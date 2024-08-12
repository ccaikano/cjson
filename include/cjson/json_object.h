//
// Created by CC on 2024/3/7.
//

#pragma once

#include <map>     // map
#include <string>  // string
#include <utility>
#include <variant>
#include <vector>  // vector

namespace cjson {

enum class JsonObjectType {
  kNull,
  kBool,
  kInt,
  kFloat,
  kNumber,
  kString,
  kArray,
  kObject,
  kInvalid
};

const char* to_string(JsonObjectType e) {
  switch (e) {
    case JsonObjectType::kNull:
      return "null";
    case JsonObjectType::kBool:
      return "bool";
    case JsonObjectType::kFloat:
      return "float";
    case JsonObjectType::kNumber:
      return "number";
    case JsonObjectType::kString:
      return "string";
    case JsonObjectType::kArray:
      return "array";
    case JsonObjectType::kObject:
      return "object";
    case JsonObjectType::kInt:
      return "int";
    case JsonObjectType::kInvalid:
      return "invalid";
  }
}
template <typename char_t = char>
class JsonObject;
using Json = JsonObject<>;

template <typename char_t>
class JsonObject {
 public:
  using null_t = std::monostate;
  using int_t = int64_t;
  using float_t = double;
  using bool_t = bool;
  using string_t = std::basic_string<char_t>;
  using array_t = std::vector<JsonObject>;
  using object_t = std::map<string_t, JsonObject>;
  using value_t =
      std::variant<null_t, bool_t, int_t, float_t, string_t, array_t, object_t>;

  explicit JsonObject(std::nullptr_t = nullptr)
      : m_value(std::monostate{}), m_type(JsonObjectType::kNull) {}

  JsonObject(const JsonObject&) = delete;
  JsonObject& operator=(const JsonObject&) = delete;
  JsonObject(JsonObject&& other) noexcept
      : m_value(std::exchange(other.m_value, null_t{})),
        m_type(std::exchange(other.m_type, JsonObjectType::kNull)) {}
  JsonObject& operator=(JsonObject&& other) noexcept {
    if (this != &other) {
      m_value = std::exchange(other.m_value, null_t{});
      m_type = std::exchange(other.m_type, JsonObjectType::kNull);
    }
    return *this;
  }

  template <typename T, std::enable_if_t<std::is_integral_v<std::decay_t<T>>, int> = 0>
  explicit JsonObject(T&& value)
      : m_value(value), m_type(JsonObjectType::kInt) {}

  template <typename T, std::enable_if_t<std::is_floating_point_v<std::decay_t<T>>, int> = 0>
  explicit JsonObject(T&& value)
      : m_value(value), m_type(JsonObjectType::kFloat) {}

  explicit JsonObject(bool_t&& value)
      : m_value(value), m_type(JsonObjectType::kBool) {}


  explicit JsonObject(const string_t& value)
      : m_value(value), m_type(JsonObjectType::kString) {}
  explicit JsonObject(string_t&& value)
      : m_value(std::move(value)), m_type(JsonObjectType::kString) {}
  explicit JsonObject(const char_t* value)
      : m_value(std::move(string_t(value))), m_type(JsonObjectType::kString) {}

  explicit JsonObject(array_t& value)
      : m_value(std::move(value)), m_type(JsonObjectType::kArray) {}
  explicit JsonObject(array_t&& value)
      : m_value(std::move(value)), m_type(JsonObjectType::kArray) {}

  explicit JsonObject(object_t& value)
      : m_value(std::move(value)), m_type(JsonObjectType::kObject) {}
  explicit JsonObject(object_t&& value)
      : m_value(std::move(value)), m_type(JsonObjectType::kObject) {}

  template <typename... args_t>
  explicit JsonObject(const JsonObjectType type, args_t&&... args)
      : m_value(std::forward<args_t>(args)...), m_type(type) {}

  [[nodiscard]] JsonObjectType type() const { return m_type; }

 private:
  value_t m_value;
  JsonObjectType m_type;
};
}  // namespace cjson

#include <cjson/macro_unscope.h>