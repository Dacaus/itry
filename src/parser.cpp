#include "parser.hpp"
#include "ast.hpp"
#include "token.hpp"
#include <map>
namespace itry {
using Op = Binary::Op;
// const std::map<Op, int> Parser::BinOpPrecedence{
//   {Op::Add, 10},
//   {Op::Subtract,10},
//   {Op::Multiply,20},
//   {Op::Divide,20}
// };

Expr Parser::parse() { return parseTerm(); }

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