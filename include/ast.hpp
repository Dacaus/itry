#pragma once

#include "itryIRBuilder.hpp"
#include "token.hpp"
#include <climits>
#include <cstdint>
#include <iostream>
#include <llvm/IR/Value.h>
#include <locale>
#include <map>
#include <memory>
#include <variant>
namespace llvm {
  class Value;
}
namespace itry {
class Number;
class Binary;
using Expr = std::variant<Number, std::unique_ptr<Binary>>;

class Number {
public:
  double value;
};

class Binary {

public:
  enum class Op {
    Add,
    Subtract,
    Multiply,
    Divide,
  };

  static const std::map<Op, std::string> op_map;

  Expr lhs;
  Expr rhs;
  Op op;
  Binary(Expr lhs, Token token, Expr rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    switch (token.getType()) {
    case TokenType::PLUS:
      op = Op::Add;
      break;
    case TokenType::MINUS:
      op = Op::Subtract;
      break;
    case TokenType::STAR:
      op = Op::Multiply;
      break;
    case TokenType::SLASH:
      op = Op::Divide;
      break;
    default:

      break;
    }
  }

  // 删除拷贝构造函数和赋值运算符
  Binary(const Binary &) = delete;
  Binary &operator=(const Binary &) = delete;

  // 允许移动
  Binary(Binary &&) = default;
  Binary &operator=(Binary &&) = default;
};

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
void print(const Expr &expr);


} // namespace itry