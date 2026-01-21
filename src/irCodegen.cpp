#include "irCodegen.hpp"
#include "ast.hpp"
#include "llvm/IR/Value.h"
#include <memory>
#include <variant>
namespace itry {
llvm::Value *IRCodegen::operator()(const Expr &expr,
                                   ItryIRBuilder &itryBuilder) {
  if (std::holds_alternative<Number>(expr)) {
    Number numberNode = std::get<Number>(expr); // 获取值
    return itryBuilder.doubleCodegen(numberNode.value);
  } else if (std::holds_alternative<std::unique_ptr<Binary>>(expr)) {
    auto leftValue = operator()(std::get<std::unique_ptr<Binary>>(expr)->lhs,
                                itryBuilder);
    auto rightValue = operator()(std::get<std::unique_ptr<Binary>>(expr)->rhs,
                                 itryBuilder);
    switch (std::get<std::unique_ptr<Binary>>(expr)->op) {
    case Binary::Op::Add:
      return itryBuilder.CreateFAdd(leftValue, rightValue, "addtmp");
    case Binary::Op::Subtract:
      return itryBuilder.CreateFSub(leftValue, rightValue, "subtmp");
    case Binary::Op::Multiply:
      return itryBuilder.CreateFMul(leftValue, rightValue, "multmp");
    case Binary::Op::Divide:
      return itryBuilder.CreateFDiv(leftValue, rightValue, "divtmp");
    default:
      throw std::runtime_error("IRCodegen: Unknown binary operator");
    }
  }
}
} // namespace itry