#include "ast.hpp"
#include <iostream>
namespace itry {

// const std::map<Binary::Op, std::string> Binary::op_map = {
//     {Op::Add, "Add"},
//     {Op::Subtract, "Subtract"},
//     {Op::Multiply, "Multiply"},
//     {Op::Divide, "Divide"}};

void AstPrinter::print(const Expr &expr, int indent) {
  std::visit([this, indent](auto &&arg) { this->print(*arg, indent); }, expr);
}

void AstPrinter::print(const Stmt &stmt, int indent) {
  std::visit([this, indent](auto &&arg) { this->print(*arg, indent); }, stmt);
}

void AstPrinter::print(const std::vector<Stmt> &stmts, int indent) {
  for (const auto &stmt : stmts) {
    print(stmt, indent);
  }
}

void AstPrinter::print(Number &number, int indent) {
  printIndent(indent);
  std::cout << "Number: " << number.value << std::endl;
}

void AstPrinter::print(Binary &binary, int indent) {
  printIndent(indent);
  std::cout << "Binary Expression: " << std::endl;
  printIndent(indent + 2);
  std::cout << "Operator: " << static_cast<int>(binary.op) << std::endl;
  printIndent(indent + 2);
  std::cout << "Left:" << std::endl;
  print(binary.left, indent + 4);
  printIndent(indent + 2);
  std::cout << "Right:" << std::endl;
  print(binary.right, indent + 4);
}

void AstPrinter::print(Identifier &identifier, int indent) {
  printIndent(indent);
  std::cout << "Identifier: " << identifier.name << std::endl;
}

void AstPrinter::print(FunctionCall &funcCall, int indent) {
  printIndent(indent);
  std::cout << "Function Call: " << funcCall.name << std::endl;
  printIndent(indent + 2);
  std::cout << "Arguments:" << std::endl;
  for (const auto &arg : funcCall.args) {
    print(arg, indent + 4);
  }
}

void AstPrinter::print(AssignStmt &assignStmt, int indent) {
  printIndent(indent);
  std::cout << "Assign Statement: " << assignStmt.name << std::endl;
  printIndent(indent + 2);
  std::cout << "Value:" << std::endl;
  print(assignStmt.value, indent + 4);
}

void AstPrinter::print(IdentifierDeclStmt &identDeclStmt, int indent) {
  printIndent(indent);
  std::cout << "Identifier Declaration: " << identDeclStmt.name << std::endl;
  if (identDeclStmt.value.has_value()) {
    printIndent(indent + 2);
    std::cout << "Value:" << std::endl;
    print(identDeclStmt.value.value(), indent + 4);
  }
}

void AstPrinter::print(FunctionDeclStmt &funcDeclStmt, int indent) {
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
  print(*funcDeclStmt.body, indent + 4);
}

void AstPrinter::print(ExpressionStmt &exprStmt, int indent) {
  printIndent(indent);
  std::cout << "Expression Statement:" << std::endl;
  print(exprStmt.expr, indent + 2);
}

void AstPrinter::print(ReturnStmt &returnStmt, int indent) {
  printIndent(indent);
  std::cout << "Return Statement:" << std::endl;
  print(returnStmt.value, indent + 2);
}

void AstPrinter::print(Block &block, int indent) {
  printIndent(indent);
  std::cout << "Block:" << std::endl;
  for (const auto &stmt : block.stmts) {
    print(stmt, indent + 2);
  }
}

void AstPrinter::printIndent(int indent) {
  for (int i = 0; i < indent; ++i) {
    std::cout << ' ';
  }
}

} // namespace itry