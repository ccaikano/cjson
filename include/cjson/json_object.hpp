//
// Created by CC on 2024/3/7.
//

#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <optional>
#include <sstream>
#include <string>  // string
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>  // vector

#include "macro_scope.hpp"
#include "type_traits.hpp"

namespace cjson {

template <typename T>
class JsonObject;
using Json = JsonObject<std::string>;
using JsonU16 = JsonObject<std::u16string>;
using wJson = JsonObject<std::wstring>;
// c++20
#if __cplusplus >= 202002L
using JsonU8 = JsonObject<std::u8string>;
#endif

template <typename string_t>
class JsonObject {
 public:
  using null_t = std::monostate;
  using int_t = int64_t;
  using float_t = double;
  using bool_t = bool;
  using array_t = std::vector<JsonObject>;
  using array_value_t = typename array_t::value_type;
  using object_t = std::unordered_map<string_t, JsonObject>;
  using object_value_t = typename object_t::value_type;
  using char_t = typename string_t::value_type;
  using value_t = std::variant<null_t, bool_t, int_t, float_t, string_t, array_t, object_t>;
  using value_type = JsonObject;
  using reference = value_type&;
  using const_reference = const value_type&;
  enum class Type { kNull, kBool, kInt, kFloat, kString, kArray, kObject, kInvalid };

  JsonObject(std::nullptr_t = nullptr) : m_value(std::monostate{}), m_type(Type::kNull) {}
  explicit JsonObject(std::monostate) : m_value(std::monostate{}), m_type(Type::kNull) {}
  JsonObject(const JsonObject& other) noexcept {
    m_value = other.m_value;
    m_type = other.m_type;
    std::cout << "copy" << std::endl;
  }
  JsonObject& operator=(const JsonObject& other) noexcept {
    m_value = other.m_value;
    m_type = other.m_type;
    std::cout << "copy" << std::endl;
    return *this;
  }
  JsonObject(JsonObject&& other) noexcept = default;
  JsonObject& operator=(JsonObject&& other) noexcept = default;
  ~JsonObject() noexcept = default;

  JsonObject(int num) : m_value(num), m_type(Type::kInt) {}
  JsonObject(unsigned num) : m_value(num), m_type(Type::kInt) {}
  JsonObject(long num) : m_value(num), m_type(Type::kInt) {}
  JsonObject(unsigned long num) : m_value(num), m_type(Type::kInt) {}
  JsonObject(long long num) : m_value(num), m_type(Type::kInt) {}
  JsonObject(unsigned long long num) : m_value(num), m_type(Type::kInt) {}
  JsonObject(float num) : m_value(num), m_type(Type::kFloat) {}
  JsonObject(double num) : m_value(num), m_type(Type::kFloat) {}
  JsonObject(long double num) : m_value(num), m_type(Type::kFloat) {}

  JsonObject(bool_t&& value) : m_value(value), m_type(Type::kBool) {}

  JsonObject(const string_t& value) : m_value(value), m_type(Type::kString) {}
  JsonObject(string_t&& value) : m_value(std::move(value)), m_type(Type::kString) {}
  JsonObject(const char_t* value) : m_value(std::move(string_t(value))), m_type(Type::kString) {}

  explicit JsonObject(array_t&& value) : m_value(std::move(value)), m_type(Type::kArray) {}

  explicit JsonObject(object_t&& value) : m_value(std::move(value)), m_type(Type::kObject) {}

  explicit JsonObject(const Type type) : m_type(type) {
    switch (type) {
      case Type::kNull:
        m_value = std::monostate{};
        break;
      case Type::kBool:
        m_value = false;
        break;
      case Type::kInt:
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
  JsonObject(std::initializer_list<JsonObject> init_list) {
    const bool is_an_object = std::all_of(init_list.begin(), init_list.end(), [](const JsonObject& element) {
      return element.isArray() && element.size() == 2 && element[0].isString();
    });
    // std::cout << is_an_object << std::endl;
    if (is_an_object) {
      // m_value = object_t(std::make_move_iterator(init_list.begin()), std::make_move_iterator(init_list.end()));
      m_type = Type::kObject;
      object_t object{};
      for (auto& pair : init_list) {
        auto& item = pair.asArray();
        object.emplace(std::move(item[0].asString()), std::move(item[1]));
      }
      m_value = std::move(object);
    } else {
      m_type = Type::kArray;
      m_value = array_t(std::make_move_iterator(init_list.begin()), std::make_move_iterator(init_list.end()));
    }
  }

  template <typename collection_t,
            std::enable_if_t<is_collection<remove_cvref_t<collection_t>> &&
                                 std::is_constructible_v<array_value_t, container_value_t<collection_t>>,
                             bool> = true>
  JsonObject(collection_t&& collection)
      : m_value(array_t(std::make_move_iterator(collection.begin()), std::make_move_iterator(collection.end()))),
        m_type(Type::kArray) {
    collection.clear();
  }

  template <typename map_t,
            std::enable_if_t<is_map<remove_cvref_t<map_t>> && std::is_constructible_v<object_value_t, container_value_t<map_t>>,
                             int> = 0>
  JsonObject(map_t&& map)
      : m_value(object_t{std::make_move_iterator(map.begin()), std::make_move_iterator(map.end())}), m_type(Type::kObject) {
    map.clear();
  }

  [[nodiscard]] Type type() const noexcept { return m_type; }

  [[nodiscard]] bool valid() const noexcept { return m_type != Type::kInvalid; }

  [[nodiscard]] bool isNull() const noexcept { return m_type == Type::kNull; }
  [[nodiscard]] bool isBool() const noexcept { return m_type == Type::kBool; }
  [[nodiscard]] bool isInt() const noexcept { return m_type == Type::kInt; }
  [[nodiscard]] bool isFloat() const noexcept { return m_type == Type::kFloat; }
  [[nodiscard]] bool isNumber() const noexcept { return isInt() || isFloat(); }
  [[nodiscard]] bool isString() const noexcept { return m_type == Type::kString; }
  [[nodiscard]] bool isArray() const noexcept { return m_type == Type::kArray; }
  [[nodiscard]] bool isObject() const noexcept { return m_type == Type::kObject; }
  template <typename value_t>
  [[nodiscard]] bool is() const noexcept {
    if constexpr (std::is_same_v<std::decay_t<value_t>, bool_t>) {
      return isBool();
    } else if constexpr (std::is_arithmetic_v<std::decay_t<value_t>>) {
      return isNumber();
    } else if constexpr (std::is_convertible_v<string_t, value_t>) {
      return isString();
    } else if constexpr (std::is_same_v<std::decay_t<value_t>, array_t>) {
      return isArray();
    } else if constexpr (is_collection<std::decay_t<value_t>>) {
      return isArray();
    } else if constexpr (std::is_same_v<std::decay_t<value_t>, object_t>) {
      return isObject();
    } else if constexpr (is_map<std::decay_t<value_t>>) {
      return isObject();
    } else {
      static_assert(false, "JsonObject::is() called with unsupported type");
    }
    return false;
  }

  [[nodiscard]] bool empty() const noexcept { return isNull(); }
  [[nodiscard]] std::size_t size() const noexcept {
    switch (m_type) {
      case Type::kNull:
        return 0;
      case Type::kBool:
      case Type::kInt:
      case Type::kFloat:
      case Type::kString:
        return 1;
      case Type::kArray:
        return std::get<array_t>(m_value).size();
      case Type::kObject:
        return std::get<object_t>(m_value).size();
      default:
        return 0;
    }
  }
  [[nodiscard]] bool asBool() const {
    if (CC_LIKELY(isBool())) {
      return std::get<bool_t>(m_value);
    }
    JSON_THROW_EXCEPTION("JsonObject::asBool() called on non-bool type")
  }
  [[nodiscard]] int_t asInt() const {
    if (CC_LIKELY(isInt())) {
      return std::get<int_t>(m_value);
    }
    JSON_THROW_EXCEPTION("JsonObject::asInt() called on non-int type")
  }
  [[nodiscard]] float_t asFloat() const {
    if (CC_LIKELY(isFloat())) {
      return std::get<float_t>(m_value);
    }
    JSON_THROW_EXCEPTION("JsonObject::asFloat() called on non-float type")
  }
  [[nodiscard]] const string_t& asString() const {
    if (CC_LIKELY(isString())) {
      return std::get<string_t>(m_value);
    }
    JSON_THROW_EXCEPTION("JsonObject::asString() called on non-string type")
  }
  [[nodiscard]] string_t& asString() {
    if (CC_LIKELY(isString())) {
      return std::get<string_t>(m_value);
    }
    JSON_THROW_EXCEPTION("JsonObject::asString() called on non-string type")
  }

  [[nodiscard]] const array_t& asArray() const {
    if (CC_LIKELY(isArray())) {
      return std::get<array_t>(m_value);
    }
    JSON_THROW_EXCEPTION("JsonObject::asArray() called on non-array type")
  }
  [[nodiscard]] array_t& asArray() {
    if (empty()) {
      *this = JsonObject(Type::kArray);
    }
    if (CC_LIKELY(isArray())) {
      return std::get<array_t>(m_value);
    }
    JSON_THROW_EXCEPTION("JsonObject::asArray() called on non-array type")
  }
  [[nodiscard]] const object_t& asObject() const {
    if (CC_LIKELY(isObject())) {
      return std::get<object_t>(m_value);
    }
    JSON_THROW_EXCEPTION("JsonObject::asObject() called on non-object type")
  }

  [[nodiscard]] object_t& asObject() {
    if (empty()) {
      *this = JsonObject(Type::kObject);
    }
    if (CC_LIKELY(isObject())) {
      return std::get<object_t>(m_value);
    }
    JSON_THROW_EXCEPTION("JsonObject::asObject() called on non-object type")
  }

  template <typename value_t = value_type>
  [[nodiscard]] const value_t& as() const {
    return as<value_t>();
  }
  template <typename value_t = value_type>
  [[nodiscard]] value_t& as() {
    if constexpr (std::is_same_v<std::decay_t<value_t>, bool_t>) {
      return asBool();
    } else if constexpr (std::is_integral_v<std::decay_t<value_t>>) {
      return asInt();
    } else if constexpr (std::is_floating_point_v<std::decay_t<value_t>>) {
      return asFloat();
    } else if constexpr (std::is_convertible_v<string_t, value_t>) {
      return static_cast<value_t>(asString());
    } else if constexpr (std::is_same_v<std::decay_t<value_t>, string_t>) {
      return asString();
    } else if constexpr (std::is_same_v<std::decay_t<value_t>, array_t>) {
      return asArray();
    } else if constexpr (std::is_same_v<std::decay_t<value_t>, object_t>) {
      return asObject();
    } else {
      return static_cast<value_t>(*this);
    }
  }

  [[nodiscard]] reference at(std::size_t index) { return asArray().at(index); }
  [[nodiscard]] const_reference at(std::size_t index) const { return asArray().at(index); }
  [[nodiscard]] reference at(const string_t& key) { return asObject().at(key); }
  [[nodiscard]] const_reference at(const string_t& key) const { return asObject().at(key); }
  [[nodiscard]] reference at(string_t&& key) { return asObject().at(std::move(key)); }
  [[nodiscard]] const_reference at(string_t&& key) const { return asObject().at(std::move(key)); }

  [[nodiscard]] bool contains(const string_t& key) const {
#if __cplusplus >= 202002L
    return asObject().contains(key);
#else
    return asObject().find(key) != asObject().end();
#endif
  }
  [[nodiscard]] bool contains(std::size_t index) const { return index < asArray().size(); }

  bool erase(const string_t& key) { return asObject().erase(key) > 0; }
  bool erase(std::size_t index) {
    if (CC_LIKELY(contains(index))) {
      asArray().erase(asArray().begin() + index);
      return true;
    }
    return false;
  }
  template <typename... args_t,std::enable_if_t<std::is_constructible_v<object_value_t, args_t...>,int> = 0>
  decltype(auto) emplace(args_t&&... args) {
    if (CC_UNLIKELY(!(isNull() || isObject()))) {
      JSON_THROW_EXCEPTION("JsonObject::emplace() called on non-object type")
    }
    return asObject().emplace(std::forward<args_t>(args)...);
  }
  template <typename... args_t,std::enable_if_t<std::is_constructible_v<array_t, args_t...>,int> = 0>
  decltype(auto) emplace(args_t&&... args) {
    if (CC_UNLIKELY(!(isNull() || isArray()))) {
      JSON_THROW_EXCEPTION("JsonObject::emplace_back() called on non-array type")
    }
    return asArray().emplace_back(std::forward<args_t>(args)...);
  }
  template <typename... args_t>
  decltype(auto) emplace_back(args_t&&... args) {
    if (CC_UNLIKELY(!(isNull() || isArray()))) {
      JSON_THROW_EXCEPTION("JsonObject::emplace_back() called on non-array type")
    }
    return asArray().emplace_back(std::forward<args_t>(args)...);
  }

  reference insert(const JsonObject& json) {
    if (CC_UNLIKELY(!(isNull() || isObject()))) {
      JSON_THROW_EXCEPTION("JsonObject::insert() called on non-object type")
    }
    asObject().insert(json.asObject().begin(), json.asObject().end());
    return *this;
  }
  reference insert(JsonObject&& json) {
    if (CC_UNLIKELY(!(isNull() || isObject()))) {
      JSON_THROW_EXCEPTION("JsonObject::insert() called on non-object type")
    }
    asObject().insert(std::make_move_iterator(json.asObject().begin()), std::make_move_iterator(json.asObject().end()));
    return *this;
  }

  void push_back(const JsonObject& json) {
    if (CC_UNLIKELY(!(isNull() || isArray()))) {
      JSON_THROW_EXCEPTION("JsonObject::push_back() called on non-array type")
    }
    asArray().push_back(json);
  }

  void push_back(JsonObject&& json) {
    if (CC_UNLIKELY(!(isNull() || isArray()))) {
      JSON_THROW_EXCEPTION("JsonObject::push_back() called on non-array type")
    }
    asArray().push_back(std::move(json));
  }

  void pop_back() {
    if (CC_UNLIKELY(!(isNull() || isArray()))) {
      JSON_THROW_EXCEPTION("JsonObject::pop_back() called on non-array type")
    }
    asArray().pop_back();
  }
  reference back() {
    if (CC_UNLIKELY(asArray().empty())) {
      JSON_THROW_EXCEPTION("JsonObject::back() called on empty array")
    }
    return asArray().back();
  }
  const_reference back() const {
    if (CC_UNLIKELY(asArray().empty())) {
      JSON_THROW_EXCEPTION("JsonObject::back() called on empty array")
    }
    return asArray().back();
  }

  reference front() {
    if (CC_UNLIKELY(asArray().empty())) {
      JSON_THROW_EXCEPTION("JsonObject::front() called on empty array")
    }
    return asArray().front();
  }

  const_reference front() const {
    if (CC_UNLIKELY(asArray().empty())) {
      JSON_THROW_EXCEPTION("JsonObject::front() called on empty array")
    }
    return asArray().front();
  }

  void clear() { *this = JsonObject(); }

  string_t dump(const std::size_t indent = 4) const { return toJsonString(indent, 0); }
  string_t toString() const { return toJsonString(4, 0); }

  bool operator==(const JsonObject& other) {
    switch (type()) {
      case Type::kNull:
        return other.isNull();
      case Type::kInt:
      case Type::kBool:
      case Type::kFloat:
      case Type::kString:
        return m_value == other.m_value;
      case Type::kArray:
        return asArray() == other.asArray();
      case Type::kObject:
        return asObject() == other.asObject();
      default:
        JSON_THROW_EXCEPTION("JsonObject::operator==() called on unknown type")
    }
  }
  bool operator!=(const JsonObject& other) { return !(*this == other); }

  [[nodiscard]] reference operator[](std::size_t index) { return asArray()[index]; }
  [[nodiscard]] const_reference operator[](std::size_t index) const { return asArray()[index]; }

  [[nodiscard]] reference operator[](const string_t& key) { return asObject()[key]; }
  [[nodiscard]] const_reference operator[](const string_t& key) const { return asObject()[key]; }

  [[nodiscard]] reference operator[](string_t&& key) { return asObject()[std::move(key)]; }
  [[nodiscard]] const_reference operator[](string_t&& key) const { return asObject()[key]; }

 private:
  string_t toJsonStringArray(std::size_t indent, std::size_t depth) const {
    const string_t body_indent_str(indent * (depth + 1), ' ');
    const string_t last_indent_str(indent * depth, ' ');
    string_t result{"[\n"};
    auto& array = asArray();
    for (auto it = array.begin(); it != array.end();) {
      result += body_indent_str + it->toJsonString(indent, depth + 1);
      if (++it != array.end()) {
        result += char_t(',');
      }
      result += char_t('\n');
    }
    result += last_indent_str + char_t(']');
    return result;
  }

  string_t toJsonStringObject(std::size_t indent, std::size_t depth) const {
    string_t result{"{\n"};
    const string_t body_indent_str(indent * (depth + 1), ' ');
    const string_t last_indent_str(indent * depth, ' ');
    auto& object = asObject();
    for (auto it = object.begin(); it != object.end();) {
      const auto& [key, value] = *it;
      result +=
          body_indent_str + char_t('\"') + key + char_t('\"') + char_t(':') + char_t(' ') + value.toJsonString(indent, depth + 1);
      if (++it != object.end()) {
        result += char_t(',');
      }
      result += char_t('\n');
    }
    result += last_indent_str + char_t('}');
    return result;
  }
  string_t toJsonString(std::size_t indent, std::size_t depth) const {
    switch (m_type) {
      case Type::kNull:
        return string_t{"null"};
      case Type::kBool:
        return asBool() ? string_t{"true"} : string_t{"false"};
      case Type::kInt:
        return to_string_generic<string_t, int_t>(asInt());
      case Type::kFloat:
        return to_string_generic<string_t, float_t>(asFloat());
      case Type::kString:
        return char_t('\"') + asString() + char_t('\"');
      case Type::kArray: {
        return toJsonStringArray(indent, depth);
      }
      case Type::kObject: {
        return toJsonStringObject(indent, depth);
      }
      default: {
        JSON_THROW_EXCEPTION("JsonObject::toJsonString() called on unknown type")
      }
    }
  }

  template <typename StringType, typename T>
  StringType to_string_generic(const T value) const {
    std::basic_ostringstream<typename StringType::value_type> oss;
    oss << value;
    return oss.str();
  }

  value_t m_value;
  Type m_type;
};
}  // namespace cjson
