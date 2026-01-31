#include "ast.hpp"
#include <iostream>
namespace itry {

struct AstPrinter {
  // class Double;
  // class Number;
  // class Binary;
  // class Identifier;
  // class FunctionCall;
  void operator()(const Expr &expr, int indent) {
    std::visit([this, indent](auto &&arg) { (*this)(*arg, indent); }, expr);
  }
  void operator()(const Double &d, int indent) {
    printIndent(indent);
    std::cout << "Double: " << d.value << std::endl;
  }
  void operator()(const Number &number, int indent) {
    printIndent(indent);
    std::cout << "Number: " << number.value << std::endl;
  }
  void operator()(const Identifier &identifier, int indent) {
    printIndent(indent);
    std::cout << "Identifier: " << identifier.name << std::endl;
  }
  void operator()(const Binary &binary, int indent) {
    printIndent(indent);
    std::cout << "Binary Expression: " << std::endl;
    printIndent(indent + 2);
    std::cout << "Operator: " << static_cast<int>(binary.op) << std::endl;
    printIndent(indent + 2);
    std::cout << "Left:" << std::endl;
    std::visit([this, indent](auto &&arg) { (*this)(*arg, indent + 4); },
               binary.left);
    printIndent(indent + 2);
    std::cout << "Right:" << std::endl;
    std::visit([this, indent](auto &&arg) { (*this)(*arg, indent + 4); },
               binary.right);
  }

  void operator()(const FunctionCall &funcCall, int indent) {
    printIndent(indent);
    std::cout << "Function Call: " << funcCall.name << std::endl;
    printIndent(indent + 2);
    std::cout << "Arguments:" << std::endl;
    for (const auto &arg : funcCall.args) {
      std::visit([this, indent](auto &&arg) { (*this)(*arg, indent + 4); },
                 arg);
    }
  }

  // class AssignStmt;
  // class IdentifierDeclStmt;
  // class FunctionDeclStmt;
  // class ExpressionStmt;
  // class ReturnStmt;

  void operator()(const AssignStmt &assignStmt, int indent) {
    printIndent(indent);
    std::cout << "Assign Statement: " << assignStmt.name << std::endl;
    printIndent(indent + 2);
    std::cout << "Value:" << std::endl;
    std::visit([this, indent](auto &&arg) { (*this)(*arg, indent + 4); },
               assignStmt.value);
  }

  void operator()(const IdentifierDeclStmt &identDeclStmt, int indent) {
    printIndent(indent);
    std::cout << "Identifier Declaration: " << identDeclStmt.name << std::endl;
    if (identDeclStmt.value.has_value()) {
      printIndent(indent + 2);
      std::cout << "Value:" << std::endl;
      std::visit([this, indent](auto &&arg) { (*this)(*arg, indent + 4); },
                 identDeclStmt.value.value());
    }
  }

  void operator()(const FunctionDeclStmt &funcDeclStmt, int indent) {
    printIndent(indent);
    std::cout << "Function Declaration: " << funcDeclStmt.name << std::endl;
    printIndent(indent + 2);
    std::cout << "Parameters:" << std::endl;
    for (const auto &param : funcDeclStmt.params) {
      printIndent(indent + 4);
      std::cout << param << std::endl;
    }
    printIndent(indent + 2);
    std::cout << "Body:" << std::endl;
    (*this)(*funcDeclStmt.body, indent + 4);
  }

  void operator()(const ExpressionStmt &exprStmt, int indent) {
    printIndent(indent);
    std::cout << "Expression Statement:" << std::endl;
    std::visit([this, indent](auto &&arg) { (*this)(*arg, indent + 2); },
               exprStmt.expr);
  }

  void operator()(const ReturnStmt &returnStmt, int indent) {
    printIndent(indent);
    std::cout << "Return Statement:" << std::endl;
    std::visit([this, indent](auto &&arg) { (*this)(*arg, indent + 2); },
               returnStmt.value);
  }

  void operator()(const Block &block, int indent) {
    printIndent(indent);
    std::cout << "Block:" << std::endl;
    for (const auto &stmt : block.stmts) {
      std::visit([this, indent](auto &&arg) { (*this)(*arg, indent + 2); },
                 stmt);
    }
  }

private:
  void printIndent(int indent) {
    for (int i = 0; i < indent; ++i) {
      std::cout << ' ';
    }
  }
};

void printAst(const std::vector<Stmt> &stmts) {
  AstPrinter printer;
  for (const auto &stmt : stmts) {
    std::visit([&printer](auto &&arg) { printer(*arg, 0); }, stmt);
  }
}
void printAst(const Stmt &stmt, int indent) {
  AstPrinter printer;
  std::visit([&printer, indent](auto &&arg) { printer(*arg, indent); }, stmt);
}
void printAst(const Expr &expr, int indent) {
  AstPrinter printer;
  std::visit([&printer, indent](auto &&arg) { printer(*arg, indent); }, expr);
}

} // namespace itry