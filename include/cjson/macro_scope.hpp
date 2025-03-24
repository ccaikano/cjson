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
#define JSON_THROW_EXCEPTION(msg)        \
  do {                                   \
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

#if defined(CC_DEBUG) && defined(IS_WIN)
#include <intrin.h>
// This is a super stable value and setting it here avoids pulling in all of windows.h.
#ifndef FAST_FAIL_FATAL_APP_EXIT
#define FAST_FAIL_FATAL_APP_EXIT 7
#endif
#endif
void cc_abort() {
#if defined(CC_DEBUG) && defined(IS_WIN)
  __fastfail(FAST_FAIL_FATAL_APP_EXIT);
#elif defined(__clang__)
  __builtin_trap();
#else
  abort();
#endif
}

#if !defined(CC_ABORT)
#if defined(IS_WIN)
// This style lets Visual Studio follow errors back to the source file.
#define CC_DUMP_LINE_FORMAT "%s(%d)"
#else
#define CC_DUMP_LINE_FORMAT "%s:%d"
#endif
#define CC_ABORT(message, ...)                                                                         \
  do {                                                                                                 \
    printf(SK_DUMP_LINE_FORMAT ": fatal error: \"" message "\"\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    cc_abort();                                                                                        \
  } while (false)
#endif

#if defined(COMPILER_GCC) || defined(__clang__)
#define CC_ASSERT_RELEASE(cond) \
  static_cast<void>(__builtin_expect(static_cast<bool>(cond), 1) ? static_cast<void>(0) : [] { CC_ABORT("check(%s)", #cond); }())

#define CC_ASSERT_F_RELEASE(cond, fmt, ...)                                                     \
  static_cast<void>(__builtin_expect(static_cast<bool>(cond), 1) ? static_cast<void>(0) : [&] { \
    CC_ABORT("assert_f(%s): " fmt, #cond, ##__VA_ARGS__);                                       \
  }())
#else
#define CC_ASSERT_RELEASE(cond) static_cast<void>((cond) ? static_cast<void>(0) : [] { CC_ABORT("check(%s)", #cond); }())

#define CC_ASSERT_F_RELEASE(cond, fmt, ...) \
  static_cast<void>((cond) ? static_cast<void>(0) : [&] { CC_ABORT("assert_f(%s): " fmt, #cond, ##__VA_ARGS__); }())
#endif

#if defined(CC_DEBUG)
#define CC_ASSERT(cond) CC_ASSERT_RELEASE(cond)
#define CC_ASSERT_F(cond, fmt, ...) CC_ASSERT_F_RELEASE(cond, fmt, ##__VA_ARGS__)
#define CC_DEBUG_FAIL(message) CC_ABORT("%s", message)
#define CC_DEBUGFAIL_F(fmt, ...) CC_ABORT(fmt, ##__VA_ARGS__)
#else
#define CC_ASSERT(cond) static_cast<void>(0)
#define CC_ASSERT_F(cond, fmt, ...) static_cast<void>(0)
#define CC_DEBUG_FAIL(message)
#define CC_DEBUGFAIL_F(fmt, ...)
#endif

// Macro for hinting that an expression is likely to be false.
#if !defined(UNLIKELY)
#if defined(COMPILER_GCC) || defined(__clang__)
#define CC_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define CC_UNLIKELY(x) (x)
#endif  // defined(COMPILER_GCC)
#endif  // !defined(UNLIKELY)

#if !defined(LIKELY)
#if defined(COMPILER_GCC) || defined(__clang__)
#define CC_LIKELY(x) __builtin_expect(!!(x), 1)
#else
#define CC_LIKELY(x) (x)
#endif  // defined(COMPILER_GCC)
#endif  // !defined(LIKELY)
