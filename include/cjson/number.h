//
// Created by CC on 2024/3/8.
//

#pragma once

#include <cstdint>
#include <cjson/type_traits.h>

namespace cc {
namespace cjson {

template<typename N = std::int64_t, typename D = double>
struct number {
  union {
    D m_double;
    N m_int;
  };
  // false 表示整型，true表示浮点型
 public:
  template<typename T, typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
  explicit number(T i) : m_int(i), type(false) {

  }

  template<typename T, typename std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
  explicit number(T d) : m_double(d), type(true) {}

  auto value() {
    if (type) {
      return m_double;
    }
    return m_int;
  }

  [[nodiscard]] bool isInteger() const {
    return !type;
  }

  [[nodiscard]] bool isFloat() const {
    return type;
  }

  [[nodiscard]] constexpr N asInteger() const {
    if (type) {
      return static_cast<N>(m_double);
    }
    return m_int;
  }

  [[nodiscard]] constexpr D asFloat() const {
    if (type) {
      return m_double;
    }
    return static_cast<D>(m_int);
  }


  // 重载小于运算符
  template<typename T, typename std::enable_if_t<is_number<T>::value, int> = 0>
  bool operator<(const T &other) const {
    if constexpr (std::is_integral_v<T>) {
      return static_cast<N>(m_int) < static_cast<N>(other);
    } else if constexpr (std::is_floating_point_v<T>) {
      return static_cast<D>(m_double) < static_cast<D>(other);
    }
  }

  // 重载大于运算符
  template<typename T>
  bool operator>(const T &other) const {
    return other < *this;
  }

  // 重载小于等于运算符
  template<typename T>
  bool operator<=(const T &other) const {
    return !(other < *this);
  }

  // 重载大于等于运算符
  template<typename T>
  bool operator>=(const T &other) const {
    return !(*this < other);
  }

  // 重载等于运算符
  template<typename T, typename std::enable_if_t<is_number<T>::value, int> = 0>
  bool operator==(const T &other) const {
    if constexpr (std::is_integral_v<T>) {
      return static_cast<N>(m_int) == static_cast<N>(other);
    } else if constexpr (std::is_floating_point_v<T>) {
      return static_cast<D>(m_double) == static_cast<D>(other);
    }
  }

  // 重载不等于运算符
  template<typename T>
  bool operator!=(const T &other) const {
    return !(*this == other);
  }

 private:
  bool type{ };
};

};

}