//
// Created by CC on 2024/3/9.
//

#pragma once

#include <iterator>
#include <type_traits>

namespace cjson {
template <typename T>
using iterator_t = decltype(std::declval<T&>().begin());
/// 移除cv和引用
template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;
/// 迭代器value_type
template <typename T>
using iter_value_t = typename std::iterator_traits<remove_cvref_t<T>>::value_type;
/// 容器value_type
template <typename R>
using container_value_t = iter_value_t<iterator_t<R>>;

/// 判断是否为string
template <typename T, typename = void>
constexpr bool is_string = false;
template <typename T>
constexpr bool is_string<T, std::void_t<typename T::traits_type>> =
    std::is_same_v<typename T::traits_type, std::char_traits<typename T::value_type>>;

/// 判断是否为container
template <typename T, typename = void>
constexpr bool is_container = false;
template <typename T>
constexpr bool is_container<T, std::void_t<typename T::value_type, container_value_t<T>>> =
    std::is_same_v<typename T::value_type, container_value_t<T>> && !is_string<T>;

/// 判断是否为map
template <typename T, typename = void>
constexpr bool is_map = false;
template <typename T>
constexpr bool is_map<T, std::void_t<typename T::key_type, typename T::mapped_type>> = is_container<T>;

/// 判断是否为fixed_array
template <typename T, typename = void>
constexpr bool is_fixed_array = false;
template <template <typename, size_t> typename arr_t, typename value_t, size_t size>
constexpr bool is_fixed_array<arr_t<value_t, size>> = true;

/// 判断是否为collection
template <typename T, typename = void>
constexpr bool is_collection = false;
template <typename T>
constexpr bool is_collection<T> = is_container<T> && !is_map<T> && !is_fixed_array<T>;

}  // namespace cjson
