//
// Created by CC on 8/11/24.
//

#pragma once

#include <string>

#include "json_object.h"
#include "macro_scope.h"

namespace cjson {
template <typename json_char_t, typename parse_t>
class Parser;

template <typename json_char_t, typename parse_t>
class Parser {
  using parse_iter_t = std::string::const_iterator;
  using json_t = JsonObject<json_char_t>;
  using string_t = typename json_t::string_t;

 public:
  Parser(parse_iter_t begin, parse_iter_t end, bool skip_comment = false)
      : m_curr(begin), m_end(end), m_skip_comment(skip_comment) {}

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

  json_t parseString() {
    auto per_pos = ++m_curr;
    string_t result{};
    while (m_curr != m_end) {
      switch (*m_curr) {
        case '"':
          return json_t(result + string_t{per_pos, m_curr});
        case '\\': {
          result += string_t{per_pos, m_curr++};
          if (m_curr == m_end) {
            return InvalidValue();
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
              if (!skip_unicode_escape(pair_high, result)) {
                return std::nullopt;
              }
              break;
            default:
              // Illegal backslash escape
              return std::nullopt;
          }
          ++m_curr;
          per_pos = m_curr;
          break;
        }
        default: {
          ++m_curr;
          break;
        }
      }
    }
    return InvalidValue();
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
    std::basic_string_view<json_char_t> token_buffer{first, m_curr};

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

  json_t fromJson() {
    json_t t = parse();
    skipWhitespace();
    if (!isValid()) {
      return InvalidValue();
    }
    return std::move(t);
  }

  [[nodiscard]] bool isValid() const { return m_curr == m_end; }

 private:
  constexpr static string_t trueString() { return {'t', 'r', 'u', 'e'}; }

  constexpr static string_t falseString() { return {'f', 'a', 'l', 's', 'e'}; }

  constexpr static string_t nullString() { return {'n', 'u', 'l', 'l'}; }

  static json_t InvalidValue() {
    return json_t{JsonObjectType::kInvalid, typename json_t::null_t{}};
    ;
  }
  json_t parse() {
    skipWhitespace();
    switch (*m_curr) {
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
      // null
      case 'n':
        if (matchString(nullString())) {
          return json_t{};
        }
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

  bool skipUnicodeEscape(string_t& result) {

  }
  parse_iter_t m_curr;
  parse_iter_t m_end;
  bool m_skip_comment;
};
}  // namespace cjson