//
// Created by CC on 2024/3/9.
//

#pragma once
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#if (defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)) && !defined(JSON_NOEXCEPTION)
#include <exception>
#define JSON_THROW(exception) throw exception
#define JSON_TRY try
#define JSON_CATCH(exception) catch (exception)
#define JSON_INTERNAL_CATCH(exception) catch (exception)
#define JSON_THROW_EXCEPTION(msg)       \
  do {                                  \
    JSON_THROW(std::runtime_error(msg)); \
  } while (0);
#else
#include <cstdlib>
#define JSON_THROW(exception) std::abort()
#define JSON_TRY if (true)
#define JSON_CATCH(exception) if (false)
#define JSON_INTERNAL_CATCH(exception) if (false)
#define JSON_THROW_EXCEPTION(msg)  \
  do {                             \
    std::cerr << msg << std::endl; \
    std::abort();                  \
  } while (0);
#endif


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

