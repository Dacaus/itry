#pragma once
#include "Token.hpp"
#include "string"
#include <cctype>
#include <charconv>
#include <iomanip>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
namespace itry {

class Lexer {
public:
  Lexer(std::string sources) : sources(sources), pos(0) {};


  std::vector<Token> scanTokens(){
    while(!isAtEnd()){
      auto token = scanToken();
      if (!token.has_value()){
        continue;
      }
      tokens.push_back(token.value());

    }
    return tokens;
  }
  
  

private:
  std::optional<Token> scanToken() {
    auto c = consume().value_or('\0');
    switch (c) {
    case '\0':
      return Token("",TokenType::_EOF,std::nullopt);
      break;
    case ' ':
      skipWhitespace();
      return std::nullopt;
      break;
    case '+':
      return createOpToken('+', TokenType::PLUS);
      break;
    case '-':
      return createOpToken('-', TokenType::MINUS);
      break;
    case '*':
      return createOpToken('*', TokenType::STAR);
      break;
    case '/':
      return createOpToken('/', TokenType::SLASH);
      break;
    default:
      return scanDoubleToken();
      break;
    }
    return std::nullopt;
  }


  Token scanDoubleToken() {
    pos--;
    char c = consume().value_or('\0');
    std::string numStr;
    while (std::isdigit(c) || c == '.') {
      numStr += c;
      c = consume().value_or('\0');
    }
    double value;
    auto [ptr, ec] =
        std::from_chars(numStr.data(), numStr.data() + numStr.size(), value);
    if ((ec != std::errc{}) || (ptr != (numStr.data()+numStr.size())))
        error("Invalid number format: " + numStr);
    return createToken(numStr,TokenType::DOUBLE, value);
  }

  Token createOpToken(char c, TokenType type) {
    return Token(c, type, std::nullopt);
  }

  Token createToken(std::string numStr,TokenType type,double value){
    return Token(numStr,type,value);
  }

  void skipWhitespace() {
    char c = consume().value();
    while (c == ' ') {
      
      c = consume().value_or('\0');
    }
    pos--;
  }

  std::optional<char> consume() { 
    if (isAtEnd())
      return std::nullopt;
    return sources[pos++]; 
  }

  bool isAtEnd() { return pos >= sources.length(); }

  void error(const std::string &str) {
    std::string ErrorStr = "Lexer: The Location: " + std::to_string(pos) + ", ";
    ErrorStr = ErrorStr + "The char: " + sources[--pos] + ", ";
    ErrorStr = ErrorStr + str;
    throw std::runtime_error(ErrorStr);
  }

private:
  std::vector<Token> tokens;
  std::string sources;
  int pos;
};

} // namespace itry