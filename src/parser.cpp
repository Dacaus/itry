#include "parser.hpp"
#include "ast.hpp"
#include <map>
namespace itry {
using Op = Binary::Op;
// const std::map<Op, int> Parser::BinOpPrecedence{
//   {Op::Add, 10},
//   {Op::Subtract,10},
//   {Op::Multiply,20},
//   {Op::Divide,20}
// };

Expr Parser::parse() {
  parseExpression();
}

Expr parseExpression() {
  auto LExpr = parseTerm();
  
}
Expr parseTerm() {}



// int Parser::GetOpPrecedence(Binary::Op op) { return BinOpPrecedence.at(op); }

} // namespace itry