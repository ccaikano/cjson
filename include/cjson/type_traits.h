//
// Created by CC on 2024/3/9.
//

#pragma once

#include <type_traits>

namespace cc::cjson {

template<typename T, typename = void>
struct is_boolean : public std::false_type {
};

template<typename T>
struct is_boolean<T, std::enable_if_t<
    std::is_same_v<bool, std::decay_t<T>>
>> : public std::true_type {
};
template<typename T>
inline constexpr bool is_boolean_v = is_boolean<T>::value;

template<typename T, typename = void>
struct is_number : public std::false_type {
};

template<typename T>
struct is_number<T, std::enable_if_t<
    !is_boolean_v<T> &&
        std::is_integral_v<T> ||
        std::is_floating_point_v<T>
>> : public std::true_type {
};

template<typename T, typename = void>
struct is_string : public std::false_type {
};

template<typename T>
struct is_string<T, std::enable_if_t<
    std::is_same_v<std::string, std::decay_t<T>> ||
        std::is_same_v<std::string_view, std::decay_t<T>>
>> : public std::true_type {
};

template<typename T, typename = void>
struct is_base_type : public std::false_type {
};
template<typename T>
struct is_base_type<T, std::enable_if_t<
    is_number<T>::value ||
        is_string<T>::value ||
        is_boolean<T>::value
>> : public std::true_type {

};

} // cc::json
