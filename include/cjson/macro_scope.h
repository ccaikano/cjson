//
// Created by CC on 2024/3/9.
//

#pragma once
#include <cstdlib>
#include <stdexcept>
#include <iostream>

// 如果定义了 JSON_EXCEPTION，则使用异常处理。否则，使用 abort()。
// #if defined(JSON_EXCEPTION)
// #define THROW_ERROR(msg)                                                  \
//   do {                                                                    \
//     throw std::logic_error(std::string(msg) + std::string(" [") +         \
//                            std::string(__FILE__ ":") +                    \
//                            std::to_string(__LINE__) + std::string("] ")); \
//   } while (0);
// #else
// #define THROW_ERROR(msg)                                                    \
//   do {                                                                      \
//     std::cerr << std::string(msg) + std::string(" [") +                     \
//                      std::string(__FILE__ ":") + std::to_string(__LINE__) + \
//                      std::string("] ")                                      \
//               << std::endl;                                                 \
//     abort();                                                                \
//   } while (0);
// #endif

// 首先，检查是否支持 C++20
#if __cplusplus >= 202002L
// 对于支持 C++20 的编译器，直接使用 [[likely]] 和 [[unlikely]]
#define CC_LIKELY(x) (x) [[likely]]
#define CC_UNLIKELY(x) (x) [[unlikely]]
#else
// 对于旧编译器，检查 GCC 或 Clang
#if defined(__GNUC__) || defined(__clang__)
// GCC 和 Clang 支持 __builtin_expect
#define CC_LIKELY(x) __builtin_expect(!!(x), 1)
#define CC_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
// 对于其他编译器，CC_LIKELY 和 CC_UNLIKELY 宏不做任何事情
    #define CC_LIKELY(x)       (x)
    #define CC_UNLIKELY(x)     (x)
#endif
#endif

