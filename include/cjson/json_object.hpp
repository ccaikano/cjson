//
// Created by CC on 2024/3/7.
//

#pragma once

#include <cstdint>
#include <map>     // map
#include <string>  // string
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>  // vector

#include "type_traits.hpp"

namespace cjson {

template <typename T>
class JsonObject;
using Json = JsonObject<std::string>;
using JsonU8 = JsonObject<std::u8string>;
using JsonU16 = JsonObject<std::u16string>;
using wJson = JsonObject<std::wstring>;

template <typename string_t>
class JsonObject {
 public:
  using null_t = std::monostate;
  using int_t = int64_t;
  using float_t = double;
  using bool_t = bool;
  using array_t = std::vector<JsonObject>;
  using object_t = std::unordered_map<string_t, JsonObject>;
  using object_value_t = typename object_t::value_type;
  using char_t = typename string_t::value_type;
  using value_t = std::variant<null_t, bool_t, int_t, float_t, string_t, array_t, object_t>;

  enum class Type { kNull, kBool, kInt, kFloat, kNumber, kString, kArray, kObject, kInvalid };

  explicit JsonObject(std::nullptr_t = nullptr) : m_value(std::monostate{}), m_type(Type::kNull) {}

  JsonObject(const JsonObject&) = delete;
  JsonObject& operator=(const JsonObject&) = delete;
  JsonObject(JsonObject&& other) noexcept
      : m_value(std::exchange(other.m_value, null_t{})), m_type(std::exchange(other.m_type, Type::kNull)) {
    std::cout << "move\n";
  }
  JsonObject& operator=(JsonObject&& other) noexcept {
    if (this != &other) {
      m_value = std::exchange(other.m_value, null_t{});
      m_type = std::exchange(other.m_type, Type::kNull);
    }
    std::cout << "move\n";

    return *this;
  }

  template <typename T, std::enable_if_t<std::is_integral_v<std::decay_t<T>>, int> = 0>
  explicit JsonObject(T&& value) : m_value(value), m_type(Type::kInt) {}

  template <typename T, std::enable_if_t<std::is_floating_point_v<std::decay_t<T>>, int> = 0>
  explicit JsonObject(T&& value) : m_value(value), m_type(Type::kFloat) {}

  explicit JsonObject(bool_t&& value) : m_value(value), m_type(Type::kBool) {}

  explicit JsonObject(const string_t& value) : m_value(value), m_type(Type::kString) {}
  explicit JsonObject(string_t&& value) : m_value(std::move(value)), m_type(Type::kString) {

  }
  explicit JsonObject(const char_t* value) : m_value(std::move(string_t(value))), m_type(Type::kString) {}

  explicit JsonObject(array_t&& value) : m_value(std::move(value)), m_type(Type::kArray) {}

  explicit JsonObject(object_t&& value) : m_value(std::move(value)), m_type(Type::kObject) {
    std::cout << "fasdf";
  }

  explicit JsonObject(const Type type) : m_type(type) {
    switch (type) {
      case Type::kNull:
        m_value = std::monostate{};
        break;
      case Type::kBool:
        m_value = false;
        break;
      case Type::kInt:
      case Type::kNumber:
        m_value = int_t{};
        break;
      case Type::kFloat:
        m_value = float_t{};
        break;
      case Type::kString:
        m_value = string_t{};
        break;
      case Type::kArray:
        m_value = array_t{};
        break;
      case Type::kObject:
        m_value = object_t{};
        break;
      default:
        m_type = Type::kInvalid;
        m_value = std::monostate{};
        break;
    }
  }

  template <typename map_t,
            std::enable_if_t<is_map<remove_cvref_t<map_t>> && std::is_constructible_v<object_value_t, container_value_t<map_t>>,
                             int> = 0>
  explicit JsonObject(map_t&& map)
      :  m_type(Type::kObject) {
    object_t object;
    std::cout << "fasd";
    for (auto it = map.begin(); it != map.end(); ++it) {
      object.emplace(std::move(it->first), JsonObject(std::move(it->second)));
    }
    m_value = std::move(object);
  }

  [[nodiscard]] Type type() const { return m_type; }

  [[nodiscard]] bool valid() const { return m_type != Type::kInvalid; }

 private:
  value_t m_value;
  Type m_type;
};
}  // namespace cjson

#include <cjson/macro_unscope.hpp>