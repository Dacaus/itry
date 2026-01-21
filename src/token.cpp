#include "token.hpp"
#include <string>
#include <unordered_map>
namespace itry {

std::unordered_map<TokenType, std::string> TokenTypeName = {
    {TokenType::FUN, "FUN"},   {TokenType::DOUBLE, "DOUBLE"},
    {TokenType::PLUS, "PLUS"}, {TokenType::MINUS, "MINUS"},
    {TokenType::STAR, "STAR"}, {TokenType::SLASH, "SLASH"},
    {TokenType::_EOF,"EOF"}
};



std::string Token::to_string() {
  std::string literal_str =
      _literal.has_value() ? std::to_string(_literal.value()) : "";
  return "Token:{\nlexeme:" + _lexeme + " ,\ntype:" + TokenTypeName.at(_type) +
         ",\nliteral: " + literal_str + " ,\n}";
}



} // namespace itry