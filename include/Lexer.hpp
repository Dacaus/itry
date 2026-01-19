#pragma once
#include "Token.hpp"
#include "string"
#include <cctype>
#include <cinttypes>
#include <queue>
#include <stdexcept>
#include <string>
#include <string_view>
namespace itry {

class Lexer {
public:
  Lexer(std::string sources) : sources(sources), cur(0) {};

  Token scanToken() {
    char c = sources[cur];
    switch (c) {
    case '+':
      return createOpToken(TokenType::PLUS);
      break;
    case '-':
      return createOpToken(TokenType::MINUS);
      break;
    case '*':
      return createOpToken(TokenType::STAR);
      break;
    case '/':
      return createOpToken(TokenType::SLASH);
      break;
    // 数字
    case ' ':
      skip_whitespace();
      break;
    default:
      if (std::isdigit(c)) {
        return scanDouble();
      } else {
        error("No matched TokenType");
      }
    }
  }

private:
  Token scanDouble() {
    char c = sources[cur];
    std::string numStr = "";
    while (std::isdigit(c)) {
      numStr += c;
    }
    return createDoubleToken(numStr, TokenType::DOUBLE);
  }

  void skip_whitespace() {
    while (sources[cur] == ' ') {
      ++cur;
    }
  }

  void error(std::string_view str) {
    std::runtime_error("LexerError: The location: " + std::to_string(cur) +
                       ",The char: " + sources[cur] + "," +
                       "The Error: " + std::string(str) + ";");
  }



  void tokenCreateError(std::string_view lexeme, const std::string &str) {
    throw std::runtime_error("lexeme: " + std::string(lexeme) + str);
  }
  Token createToken(std::string lexeme, TokenType type,
                    std::optional<double> literal) {
    return Token(lexeme, type, literal);
  };

  Token createDoubleToken(std::string_view lexeme, TokenType type) {
    double value;
    auto [ptr, ec] =
        std::from_chars(lexeme.data(), lexeme.data() + lexeme.size(), value);
    if (ec != std::errc()) { // 解析失败
      if (ec == std::errc::invalid_argument) {
        tokenCreateError(lexeme, "不是一个有效的数字");
      } else if (ec == std::errc::result_out_of_range) {
        tokenCreateError(lexeme, "数值超出double表示范围");
      } else {
        tokenCreateError(lexeme, "未知的数字解析错误");
      }
    }

    if (ptr != lexeme.data() + lexeme.size()) { // 没有完全解析
      char invalid_char = *ptr;
      tokenCreateError(lexeme, "包含无效字符 " + std::string(1, invalid_char));
    }
    return createToken(std::string(lexeme), TokenType::DOUBLE, value);
  }

  Token createOpToken(TokenType type) {
    return createToken(std::string(1,sources[cur]), type, std::nullopt);
  }

private:
  std::string sources;
  int cur;
};

} // namespace itry