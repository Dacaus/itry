#pragma once

#include "Token.hpp"
#include <climits>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <variant>
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
  Binary(Expr lhs, Expr rhs, Token token)
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
};

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void print(const Expr &expr);

} // namespace itry