//
// Created by CC on 8/11/24.
//

#pragma once

#include <string>

#include "json_object.h"
#include "macro_scope.h"

#include <cstdint>
#include <optional>

namespace cjson {
template <typename string_t, typename parse_t>
class Parser;

template <typename string_t, typename parse_t>
class Parser {
  using parse_iter_t = std::string::const_iterator;
  using json_t = JsonObject<string_t>;
  using char_t = typename string_t::value_type;

 public:
  Parser(parse_iter_t begin, parse_iter_t end, bool skip_comment = false)
      : m_curr(begin), m_end(end), m_skip_comment(skip_comment) {}

  json_t fromJson() {
    json_t t = parseValue();
    skipWhitespace();
    if (!isValid()) {
      return InvalidValue();
    }
    return std::move(t);
  }

 private:
  [[nodiscard]] bool isValid() const { return m_curr == m_end; }

  bool matchString(const string_t& target) {
    for (const auto& ch : target) {
      if (m_curr != m_end && *m_curr == ch) {
        ++m_curr;
      } else {
        return false;
      }
    }
    return true;
  }

  json_t parseBool() {
    if (matchString(trueString())) {
      return json_t{true};
    }
    if (matchString(falseString())) {
      return json_t{false};
    }
    return InvalidValue();
  }

  json_t parseObject() {
    ++m_curr;
    typename json_t::object_t object{};
    if (!skipWhitespace()) {
      return InvalidValue();
    } else if (*m_curr == '}') {
      ++m_curr;
      return json_t(std::move(object));
    }

    while (true) {
      if (!skipWhitespace()) {
        return InvalidValue();
      }
      auto key = parseStdString();
      if (key && skipWhitespace() && *m_curr == ':') {
        ++m_curr;
      } else {
        return InvalidValue();
      }
      if (!skipWhitespace()) {
        return InvalidValue();
      }
      json_t val = parseValue();

      if (!val.valid() || !skipWhitespace()) {
        return InvalidValue();
      }
      object.emplace(std::move(*key), std::move(val));

      if (*m_curr == ',') {
        ++m_curr;
      } else {
        break;
      }
    }
    if (skipWhitespace() && *m_curr == '}') {
      ++m_curr;
    } else {
      return InvalidValue();
    }

    return json_t(std::move(object));
  }
  json_t parseArray() {
    ++m_curr;
    typename json_t::array_t array{};
    if (!skipWhitespace()) {
      return InvalidValue();
    } else if (*m_curr == ']') {
      ++m_curr;
      return json_t(std::move(array));
    }

    while (true) {
      json_t val = parseValue();

      if (!val.valid() || !skipWhitespace()) {
        return InvalidValue();
      }

      array.emplace_back(std::move(val));

      if (*m_curr == ',') {
        ++m_curr;
      } else {
        break;
      }
    }
    if (skipWhitespace() && *m_curr == ']') {
      ++m_curr;
    } else {
      return InvalidValue();
    }

    return json_t(std::move(array));
  }
  json_t parseString() {
    auto str = parseStdString();
    if (!str) {
      return InvalidValue();
    }
    return json_t(std::move(str.value()));
  }
  std::optional<string_t> parseStdString() {
    auto per_pos = ++m_curr;
    string_t result{};
    uint16_t high_surrogate_pair = 0;
    while (m_curr != m_end) {
      switch (*m_curr) {
        case '"':
          if (high_surrogate_pair) {
            return std::nullopt;
          }
          return result + string_t{per_pos, m_curr++};
        case '\\': {
          result += string_t{per_pos, m_curr++};
          if (m_curr == m_end) {
            return std::nullopt;
          }
          if (high_surrogate_pair != 0 && *m_curr != 'u') {
            return std::nullopt;
          }
          switch (*m_curr) {
            case '"':
              result.push_back('"');
              break;
            case '\\':
              result.push_back('\\');
              break;
            case '/':
              result.push_back('/');
              break;
            case 'b':
              result.push_back('\b');
              break;
            case 'f':
              result.push_back('\f');
              break;
            case 'n':
              result.push_back('\n');
              break;
            case 'r':
              result.push_back('\r');
              break;
            case 't':
              result.push_back('\t');
              break;
            case 'u':
              if (!skipUnicodeEscape(high_surrogate_pair, result)) {
                return std::nullopt;
              }
              break;
            default:
              // 非法转义
              return std::nullopt;
          }
          per_pos = ++m_curr;
          break;
        }
        default: {
          if (high_surrogate_pair) {
            return std::nullopt;
          }
          ++m_curr;
          break;
        }
      }
    }
    return std::nullopt;
  }

  json_t parseNumber() {
    // 负数
    const auto first = m_curr;
    if (*m_curr == '-') {
      ++m_curr;
    }

    // 除浮点数，其它不能包含前导0
    if (*m_curr == '0' && m_curr + 1 != m_end && std::isdigit(*(m_curr + 1))) {
      return InvalidValue();
    }
    if (!skipDigit()) {
      return InvalidValue();
    }
    // 小数点
    if (*m_curr == '.') {
      ++m_curr;
      if (!skipDigit()) {
        return InvalidValue();
      }
    }

    // 指数
    if (*m_curr == 'e' || *m_curr == 'E') {
      if (++m_curr == m_end) {
        return InvalidValue();
      }
      if (*m_curr == '+' || *m_curr == '-') {
        ++m_curr;
      }
      if (!skipDigit()) {
        return InvalidValue();
      }
    }
    char* endptr{};
    std::basic_string_view<char_t> token_buffer{first, m_curr};

    const double float_number = std::strtod(token_buffer.data(), &endptr);
    if (endptr != token_buffer.end()) {
      return InvalidValue();
    }
    if (errno == 0) {
      auto cast_number = static_cast<typename json_t::int_t>(float_number);
      if (cast_number == float_number) {
        return json_t(cast_number);
      }
      return json_t(float_number);
    }
    return InvalidValue();
  }
  constexpr static string_t trueString() { return {'t', 'r', 'u', 'e'}; }

  constexpr static string_t falseString() { return {'f', 'a', 'l', 's', 'e'}; }

  constexpr static string_t nullString() { return {'n', 'u', 'l', 'l'}; }

  static json_t InvalidValue() {
    return json_t{json_t::Type::kInvalid, typename json_t::null_t{}};
    ;
  }
  json_t parseValue() {
    skipWhitespace();
    switch (*m_curr) {
      // null
      case 'n':
        if (matchString(nullString())) {
          return json_t{};
        }
      // bool
      case 't':
      case 'f':
        return parseBool();
      // number
      case '-':
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        return parseNumber();
      // string
      case '\"':
        return parseString();
      case '[':
        return parseArray();
      case '{':
        return parseObject();

      default:
        return json_t{};
    }
  }
  /**
   * @return true 表示还有内容，false表示没内容
   */
  bool skipWhitespace() {
    auto current = *m_curr;
    while (m_curr != m_end && (current == ' ' || current == '\t' ||
                               current == '\n' || current == '\r')) {
      ++m_curr;
      current = *m_curr;
    }
    return m_curr != m_end;
  }

  bool skipDigit() {
    // 至少一位
    if (m_curr != m_end && std::isdigit(*m_curr)) {
      ++m_curr;
    } else {
      return false;
    }
    while (m_curr != m_end && std::isdigit(*m_curr)) {
      ++m_curr;
    }
    return true;
  }

  bool skipUnicodeEscape(uint16_t& high_surrogate_pair, string_t& result) {
    uint16_t unicode_vlaue = 0;
    // 提取转义unicode value
    for (int i = 0; i < 4; ++i) {
      if (++m_curr == m_end) {
        return false;
      }
      if (!std::isxdigit(static_cast<uint8_t>(*m_curr))) {
        return false;
      }
      unicode_vlaue <<= 4;

      if ('0' <= *m_curr && *m_curr <= '9') {
        unicode_vlaue |= *m_curr - '0';
      } else if ('a' <= *m_curr && *m_curr <= 'f') {
        unicode_vlaue |= *m_curr - 'a' + 10;
      } else if ('A' <= *m_curr && *m_curr <= 'F') {
        unicode_vlaue |= *m_curr - 'A' + 10;
      } else {
        return false;
      }
    }

    uint32_t ext_cp = unicode_vlaue;
    uint16_t hi_cp = 0, lo_cp = 0;

    // 判断是不是在高代理位
    if (0xD800 <= unicode_vlaue && unicode_vlaue <= 0xDBFF) {
      if (high_surrogate_pair) {
        return false;
      }
      high_surrogate_pair = unicode_vlaue;
      return true;
    }
    // 低代理位
    if (0xDC00 <= unicode_vlaue && unicode_vlaue <= 0xDFFF) {
      if (!high_surrogate_pair) {
        return false;
      }
      // 高代理位+低代理位转换成真正的unicode
      ext_cp =
          (((high_surrogate_pair - 0xD800) << 10) | (unicode_vlaue - 0xDC00)) +
          0x10000;
      hi_cp = high_surrogate_pair;
      lo_cp = unicode_vlaue;
      high_surrogate_pair = 0;
    }
    //
    if constexpr (std::is_same_v<typename string_t::value_type, char>) {
      // utf8
      if (ext_cp <= 0x7F) {
        result.push_back(static_cast<char>(ext_cp));
      } else if (ext_cp <= 0x7FF) {
        result.push_back(
            static_cast<char>(((ext_cp >> 6) & 0b00011111) | 0b11000000u));
        result.push_back(
            static_cast<char>((ext_cp & 0b00111111) | 0b10000000u));
      } else if (ext_cp <= 0xFFFF) {
        result.push_back(
            static_cast<char>(((ext_cp >> 12) & 0b00001111) | 0b11100000u));
        result.push_back(
            static_cast<char>(((ext_cp >> 6) & 0b00111111) | 0b10000000u));
        result.push_back(
            static_cast<char>((ext_cp & 0b00111111) | 0b10000000u));
      } else {
        result.push_back(
            static_cast<char>(((ext_cp >> 18) & 0b00000111) | 0b11110000u));
        result.push_back(
            static_cast<char>(((ext_cp >> 12) & 0b00111111) | 0b10000000u));
        result.push_back(
            static_cast<char>(((ext_cp >> 6) & 0b00111111) | 0b10000000u));
        result.push_back(
            static_cast<char>((ext_cp & 0b00111111) | 0b10000000u));
      }
    } else if constexpr (std::is_same_v<typename string_t::value_type,
                                        wchar_t>) {
      if constexpr (sizeof(wchar_t) == 4) {
        result.push_back(static_cast<wchar_t>(ext_cp));
      } else if constexpr (sizeof(wchar_t) == 2) {
        if (ext_cp <= 0xFFFF) {
          result.push_back(static_cast<wchar_t>(ext_cp));
        } else {
          result.push_back(static_cast<wchar_t>(hi_cp));
          result.push_back(static_cast<wchar_t>(lo_cp));
        }
      } else {
        static_assert(!sizeof(typename string_t::value_type),
                      "Unsupported wchar");
      }
    } else {
      static_assert(!sizeof(typename string_t::value_type), "Unsupported type");
    }

    return true;
  }
  parse_iter_t m_curr;
  parse_iter_t m_end;
  bool m_skip_comment;
};
}  // namespace cjson