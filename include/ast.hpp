#pragma once
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>
namespace llvm {
class Value;
}
namespace itry {

class Number;
class Binary;
class Identifier;
class FunctionCall;

using Expr =
    std::variant<std::unique_ptr<Number>, std::unique_ptr<Binary>,
                 std::unique_ptr<FunctionCall>, std::unique_ptr<Identifier>>;

class Number {
public:
  Number(double value) : value(value) {}
  double value;
};

class Binary {
public:
  enum class Op { ADD, SUB, MUL, DIV };
  Binary(Expr left, Op op, Expr right)
      : left(std::move(left)), op(op), right(std::move(right)) {}
  Expr left;
  Op op;
  Expr right;
};

class Identifier {
public:
  Identifier(std::string name) : name(name) {}
  std::string name;
};

class FunctionCall {
public:
  FunctionCall(std::string name, std::vector<Expr> args)
      : name(name), args(std::move(args)) {}
  std::string name;
  std::vector<Expr> args;
};

class AssignStmt;
class IdentifierDeclStmt;
class FunctionDeclStmt;
class ExpressionStmt;
class ReturnStmt;
using Stmt = std::variant<
    std::unique_ptr<AssignStmt>, std::unique_ptr<IdentifierDeclStmt>,
    std::unique_ptr<FunctionDeclStmt>, std::unique_ptr<ExpressionStmt>,
    std::unique_ptr<ReturnStmt>>;
class Block;
class Block {
public:
  Block(std::vector<Stmt> stmts) : stmts(std::move(stmts)) {}
  std::vector<Stmt> stmts;
};

class AssignStmt {
public:
  AssignStmt(std::string name, Expr value)
      : name(name), value(std::move(value)) {}
  std::string name;
  Expr value;
};

class IdentifierDeclStmt {
public:
  IdentifierDeclStmt(std::string name, Expr value)
      : name(name), value(std::move(value)) {}
  IdentifierDeclStmt(std::string name) : name(name), value(std::nullopt) {}
  std::string name;
  std::optional<Expr> value;
};

class FunctionDeclStmt {
public:
  FunctionDeclStmt(std::string name, std::vector<std::string> params,
                   std::unique_ptr<Block> body)
      : name(name), params(std::move(params)), body(std::move(body)) {}
  std::string name;
  std::vector<std::string> params;
  std::unique_ptr<Block> body;
};

class ReturnStmt {
public:
  ReturnStmt(Expr value) : value(std::move(value)) {}
  Expr value;
};

class ExpressionStmt {
public:
  ExpressionStmt(Expr expr) : expr(std::move(expr)) {}
  Expr expr;
};

// A class to print the AST included all of AST'type for debugging purposes
class AstPrinter {
public:
  void print(const Expr &expr, int indent = 0);
  void print(const Stmt &stmt, int indent = 0);
  void print(const std::vector<Stmt> &stmts, int indent = 0);
  void print(Number &number, int indent = 0);
  void print(Binary &binary, int indent = 0);
  void print(Identifier &identifier, int indent = 0);
  void print(FunctionCall &funcCall, int indent = 0);
  void print(AssignStmt &assignStmt, int indent = 0);
  void print(IdentifierDeclStmt &identDeclStmt, int indent = 0);
  void print(FunctionDeclStmt &funcDeclStmt, int indent = 0);
  void print(ExpressionStmt &exprStmt, int indent = 0);
  void print(ReturnStmt &returnStmt, int indent = 0);
  void print(Block &block, int indent = 0);

private:
  void printIndent(int indent);
};

} // namespace itry