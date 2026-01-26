#include "token.hpp"
#include <string>
#include <unordered_map>
namespace itry {

std::unordered_map<TokenType, std::string> TokenTypeName = {
    {TokenType::FUN, "FUN"},
    {TokenType::DOUBLE, "DOUBLE"},
    {TokenType::PLUS, "PLUS"},
    {TokenType::MINUS, "MINUS"},
    {TokenType::STAR, "STAR"},
    {TokenType::SLASH, "SLASH"},
    {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::EQUAL, "EQUAL"},
    {TokenType::LEFT_PAREN, "LEFT_PAREN"},
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::_EOF, "EOF"}};


const std::unordered_map<std::string, TokenType> ReservedWords = {
    {"func", TokenType::FUN},
};

std::string Token::to_string() {
  std::string literal_str =
      _literal.has_value() ? std::to_string(_literal.value()) : "";
  return "Token:{\nlexeme:" + _lexeme + " ,\ntype:" + TokenTypeName.at(_type) +
         ",\nliteral: " + literal_str + " ,\n}";
}

} // namespace itry