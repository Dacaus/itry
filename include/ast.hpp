#pragma once
#include "token.hpp"
#include <llvm/IR/Value.h>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <iostream>
namespace llvm {
class Value;
}
namespace itry {
class Number;
class Binary;
class Assignment;
class Function;
using Expr = std::variant<Number, std::unique_ptr<Binary>, std::unique_ptr<Assignment>, std::unique_ptr<Function>>;

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


class Assignment {
  public:
    std::string identifier;
    Expr value;
    Assignment(std::string_view identifier, Expr value)
      : identifier(std::string(identifier)), value(std::move(value)) {}
};

class Function{
  public:
    std::string name;
    std::vector<std::string> params;

    Expr body;
    Function(std::string_view name, std::vector<std::string> params, Expr body)
      : name(std::string(name)), params(std::move(params)), body(std::move(body)) {}
};

class AstPrinter {
public:
  void print(const Expr &expr){
    if (std::holds_alternative<Number>(expr)){
      printNumber(std::get<Number>(expr));
    }
    else if(std::holds_alternative<std::unique_ptr<Binary>>(expr)){
      printBinary(*std::get<std::unique_ptr<Binary>>(expr));
    }
    else if(std::holds_alternative<std::unique_ptr<Assignment>>(expr)){
      printAssignment(*std::get<std::unique_ptr<Assignment>>(expr));
    }
    else if(std::holds_alternative<std::unique_ptr<Function>>(expr)){
      printFunction(*std::get<std::unique_ptr<Function>>(expr));
    }
  }

private:
  void printBinary(const Binary &binary){
    std::cout << "(";
    print(binary.lhs);
    std::cout << " " << Binary::op_map.at(binary.op) << " ";
    print(binary.rhs);
    std::cout << ")";
  }
  void printNumber(const Number &number){
    std::cout << number.value;
  }
  void printAssignment(const Assignment &assignment){
    std::cout << assignment.identifier << " = ";
    print(assignment.value);
  }
  void printFunction(const Function &function){
    std::cout << "fn " << function.name << "(";
    for(size_t i = 0; i < function.params.size(); ++i){
      std::cout << function.params[i];
      if(i != function.params.size() -1){
        std::cout << ", ";
      }
    }
    std::cout << ") {\n";
    print(function.body);
    std::cout << "\n}";
  }
};

} // namespace itry