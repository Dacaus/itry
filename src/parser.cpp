#include "parser.hpp"
#include "ast.hpp"
#include "token.hpp"
namespace itry {
using Op = Binary::Op;

Expr Parser::parse() { return parseExpression(); }

Expr Parser::parseExpression() {
  if (match({TokenType::DOUBLE})) {
    return parseTerm();
  }
  else if(match({TokenType::IDENTIFIER})){
    return assignment();
  }
  throw std::runtime_error("Parser: Unexpected token in expression");
}

Expr Parser::assignment() {
  auto identifier_token = advance();
  if (match({TokenType::EQUAL})) {
    advance(); // consume '='
    auto value_expr = parseExpression();
    return std::make_unique<Assignment>(identifier_token.getLexeme(), std::move(value_expr));
  }
  throw std::runtime_error("Parser: Expected '=' after identifier");
}


Expr Parser::parseTerm() {
  auto Left = parseFactor();
  while (match({TokenType::PLUS, TokenType::MINUS})) {
    auto op_token = advance();

    auto Right = parseFactor();
    Left =  std::make_unique<Binary>(std::move(Left), op_token,
                                    std::move(Right));
  }
  return Left;
}

Expr Parser::parseFactor() {
  auto Left = parseDouble();
  while (match({TokenType::STAR, TokenType::SLASH})) {
    auto op_token = advance();

    auto Right = parseDouble();
    Left =  std::make_unique<Binary>(std::move(Left), op_token,
                                    std::move(Right));
  }
  return Left;
}

Expr Parser::parseDouble() {
  if (match({TokenType::DOUBLE})) {
    auto token = advance();
    Number num;
    num.value = token.getLiteral().value();
    return num;
  }
  throw std::runtime_error("Parser: Expected a number");
}

// int Parser::GetOpPrecedence(Binary::Op op) { return BinOpPrecedence.at(op); }

} // namespace itry