//
// Created by CC on 2024/3/7.
//

#pragma once
#include <cstdint> // uint8_t

namespace cc {
namespace cjson {
/**
 * @brief json type enum
 */
enum class value_t : std::uint8_t {
  null,       /// 空值
  object,     /// 对象
  array,      /// 数组
  string,     /// 字符串
  boolean,    /// 布尔
  number,     /// 数字
};

} // cjson
} // cc
