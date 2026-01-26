#pragma once
#include "token.hpp"
#include <iostream>
#include <llvm/IR/Value.h>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
namespace llvm {
class Value;
}
namespace itry {
class Number;
class Binary;
class Variable;
class Assignment;
class FunctionExpr;
using Expr =
    std::variant<Number, std::unique_ptr<Binary>, std::unique_ptr<Assignment>,
                 std::unique_ptr<FunctionExpr>, std::unique_ptr<Variable>>;

class Number {
  double value;

public:
  Number(double value) : value(value) {}
};

class Variable {
  std::string name;

public:
  Variable(std::string name) : name(name) {}
};

class Assignment {
  std::string name;
  Expr value;

public:
  Assignment(std::string name, Expr value)
      : name(name), value(std::move(value)) {}
};

class Stmt {
  Expr expr;

public:
  Stmt(Expr expr) : expr(std::move(expr)) {}
};

class Block {
  std::vector<Stmt> statements;

public:
  Block() = default;
  Block(std::vector<Stmt> statements) : statements(std::move(statements)) {}
  void addStatement(Stmt stmt) { statements.push_back(std::move(stmt)); }
};

} // namespace itry