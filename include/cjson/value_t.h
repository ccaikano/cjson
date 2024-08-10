//
// Created by CC on 2024/3/7.
//

#pragma once
#include <cstdint> // uint8_t


namespace cc::cjson {
/**
 * @brief json type enum
 */
enum class value_t : std::uint8_t {
  Null, /// 空值
  Object, /// 对象
  Array, /// 数组
  String, /// 字符串
  Boolean, /// 布尔
  Number, /// 数字
};
} // cc::cjson