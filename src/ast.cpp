#include "ast.hpp"

#include "itryIRBuilder.hpp"
namespace itry {

const std::map<Binary::Op, std::string> Binary::op_map = {
    {Op::Add, "Add"},
    {Op::Subtract, "Subtract"},
    {Op::Multiply, "Multiply"},
    {Op::Divide, "Divide"}};

void print(const Expr &expr) {
  std::visit(overloaded{[](const Number &n) { std::cout << n.value; },
                        [](const std::unique_ptr<Binary> &b) {
                          std::cout << "(";
                          print(b->lhs);
                          std::cout << " " << Binary::op_map.at(b->op) << " ";
                          print(b->rhs);
                          std::cout << ")";
                        }},

             expr);
}





} // namespace itry