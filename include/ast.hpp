#pragma once
#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>
namespace llvm {
class Value;
}
namespace itry {

class Double;
class Number;
class Binary;
class Identifier;
class FunctionCall;
// class StructExpr;

using Expr =
    std::variant< std::unique_ptr<Double>,
                 std::unique_ptr<Number>, std::unique_ptr<Binary>,
                 
                 std::unique_ptr<FunctionCall>, std::unique_ptr<Identifier>>;

class Double {
public:
  Double(double value) : value(value) {}
  double value;
};

// class StructExpr {
// public:
//   StructExpr(std::string name, std::vector<Expr> members)
//       : name(name), members(std::move(members)) {}
//   std::string name;
//   std::vector<Expr> members;
// };


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
using Stmt =
    std::variant<std::unique_ptr<AssignStmt>,
                 std::unique_ptr<IdentifierDeclStmt>,
                 std::unique_ptr<FunctionDeclStmt>,
                 std::unique_ptr<ExpressionStmt>, std::unique_ptr<ReturnStmt>>;
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

void printAst(const std::vector<Stmt> &stmts);
void printAst(const Stmt &stmt, int indent = 0);
void printAst(const Expr &expr, int indent = 0);

} // namespace itry