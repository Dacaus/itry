#pragma once
#include "string"
#include "token.hpp"
#include <cctype>
#include <charconv>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#ifdef _DEBUG
#include <iostream>
#endif
namespace itry {

class Lexer {
public:
  Lexer(std::string_view sources) : sources(sources), pos(0), reservedWordsTable(GlobalReservedWordsTable::getInstance()) {};

  std::vector<Token> scanTokens() {
    while (!isAtEnd()) {
      auto token = scanToken();
      if (token.has_value()) {
        tokens.push_back(token.value());
#ifdef _DEBUG
        std::cout << "Scanned Token: " << token->to_string() << std::endl;
#endif
      } else {
        break;
      }
    }
    return tokens;
  }

private:
  std::optional<Token> scanToken() {
    auto c = consume();
    if (c == '\0') {
      return std::nullopt;
    }
#ifdef _DEBUG
    std::cout << "scanToken() char: " << c << std::endl;
#endif
    switch (c) {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
      skipWhitespace();
      return scanToken();

    case '+':
      return createOpToken('+', TokenType::PLUS);
    case '-':
      return createOpToken('-', TokenType::MINUS);
    case '*':
      return createOpToken('*', TokenType::STAR);
    case '/':
      return createOpToken('/', TokenType::SLASH);
    case '=':
      return createOpToken('=', TokenType::EQUAL);
    case '(':
      return createOpToken('(', TokenType::LEFT_PAREN);
    case ')':
      return createOpToken(')', TokenType::RIGHT_PAREN);
    case ',':
      return createOpToken(',', TokenType::COMMA);
    case '{':
      return createOpToken('{', TokenType::LEFT_BRACE);
    case '}':
      return createOpToken('}', TokenType::RIGHT_BRACE);
    case ';':
      return createOpToken(';', TokenType::SEMICOLON);
    default:
      if (std::isalpha(c)) {
        backtrack();
        return scanIdentifierToken();
      } else if (std::isdigit(c) || c == '.') {
        backtrack();
        return scanDoubleToken();
      } else {
        error("Lexer:Unexpected character.");
      }
    }

    return std::nullopt;
  }

  Token scanIdentifierToken() {
    std::string idStr;
    while (true) {
      auto c = consume();
#ifdef _DEBUG
      std::cout << "Identifier char: " << c << std::endl;
#endif
      if (c != '\0' && (std::isalnum(c) || c == '_')) {
        idStr.push_back(c);
      } else if (c == '\0') {
        break;
      } else {
        backtrack();
        break;
      }
    }

    if (reservedWordsTable.getTokenType(idStr).has_value()) {
      return Token(idStr, reservedWordsTable.getTokenType(idStr).value(), std::nullopt);
    }

    return createIdentiferToken(idStr);
  }

  Token scanDoubleToken() {
    std::string numStr;
    int dotCount = 0;
    while (true) {
      auto c = consume();
      if (c != '\0' && (std::isdigit(c) || c == '.')) {
        if (c == '.') {
          dotCount++;
          if (dotCount > 1) {
            error("Lexer: Invalid number format with multiple dots.");
          }
        }
        numStr.push_back(c);
      } else {
        break;
      }
    }
    backtrack();
    double value = 0.0;
    std::from_chars(numStr.data(), numStr.data() + numStr.size(), value);
    return createToken(numStr, TokenType::DOUBLE, value);
  }

  Token createOpToken(char c, TokenType type) {
    return Token(c, type, std::nullopt);
  }

  Token createIdentiferToken(std::string idStr) {
    return Token(idStr, TokenType::IDENTIFIER, std::nullopt);
  }

  Token createToken(std::string numStr, TokenType type, double value) {
    return Token(numStr, type, value);
  }

  void skipWhitespace() {
    while (true) {
      auto c = consume();
      if (c == '\0') {
        break;
      }
      if (c == ' ' || c == '\r' || c == '\t' || c == '\n') {
        continue;
      } else {
        backtrack();
        break;
      }
    }
  }

  char consume() {
    if (isAtEnd())
      return '\0';
    return sources[pos++];
  }

  char peek() {
    if (isAtEnd())
      return '\0';
    return sources[pos];
  }

  void backtrack() {
    if (pos > 0)
      --pos;
  }

  bool isAtEnd() { return pos >= sources.length(); }

  void error(const std::string &str) {
    std::string ErrorStr = "Lexer: The Location: " + std::to_string(pos) + ", ";
    ErrorStr = ErrorStr + "The char: " + sources[--pos] + ", ";
    ErrorStr = ErrorStr + str;
    throw std::runtime_error(ErrorStr);
  }

private:
  GlobalReservedWordsTable &reservedWordsTable;
  std::vector<Token> tokens;
  std::string sources;
  int pos;
};

} // namespace itry