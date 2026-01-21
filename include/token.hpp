#pragma once
#include "string"
#include <optional>

namespace itry {

enum class TokenType {
  FUN,
  DOUBLE,
  PLUS,
  MINUS,
  STAR,
  SLASH,
  _EOF,
};

class Token {
public:
  std::string to_string();

  std::string getLexeme() { return _lexeme; }
  TokenType getType() { return _type; }
  std::optional<double> getLiteral() { return _literal; }


  Token(std::string lexeme, TokenType type, std::optional<double> literal)
      : _lexeme(std::string(lexeme)), _type(type), _literal(literal) {};
  Token(char c, TokenType type, std::optional<double> literal)
      : _lexeme(std::string(1,c)), _type(type), _literal(literal) {};
private:
  std::string _lexeme;
  TokenType _type;
  std::optional<double> _literal;
};

} // namespace itry