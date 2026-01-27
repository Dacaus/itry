#include "token.hpp"
#include <string>
#include <unordered_map>
namespace itry {

std::unordered_map<TokenType, std::string> TokenTypeName = {
    {TokenType::FUNC, "FUNC"},
    {TokenType::VAR, "VAR"},
    {TokenType::RETURN, "RETURN"},
    {TokenType::DOUBLE, "DOUBLE"},
    {TokenType::PLUS, "PLUS"},
    {TokenType::MINUS, "MINUS"},
    {TokenType::STAR, "STAR"},
    {TokenType::SLASH, "SLASH"},
    {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::EQUAL, "EQUAL"},
    {TokenType::LEFT_PAREN, "LEFT_PAREN"},
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenType::LEFT_BRACE, "LEFT_BRACE"},
    {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
    {TokenType::SEMICOLON, "SEMICOLON"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::_EOF, "EOF"}};

const std::unordered_map<std::string, TokenType> ReservedWords = {
    {"func", TokenType::FUNC},
    {"var", TokenType::VAR},
    {"return", TokenType::RETURN}
};

std::string Token::to_string() {
  std::string literal_str =
      _literal.has_value() ? std::to_string(_literal.value()) : "";
  return "Token:{\nlexeme:" + _lexeme + " ,\ntype:" + TokenTypeName.at(_type) +
         ",\nliteral: " + literal_str + " ,\n}";
}

} // namespace itry