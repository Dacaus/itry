#include "parser.hpp"
#include "ast.hpp"
#include "token.hpp"
#include <cerrno>
#include <cmath>
#include <memory>
#include <optional>
#include <utility>
#include <vector>
namespace itry {
std::vector<Stmt> Parser::parse(std::vector<Token> tokens) {
  this->tokens = tokens;
  this->pos = 0;
  return parseProgram();
}

std::vector<Stmt> Parser::parseProgram() {
  std::vector<Stmt> statements;
  while (!isAtEnd()) {
    statements.push_back(parseStmt());
  }
  return statements;
}

std::unique_ptr<Block> Parser::parseBlock() {
  consume(TokenType::LEFT_BRACE, "Expected '{' at the beginning of block.");
  std::vector<Stmt> states;
  while (!isAtEnd() && peek().getType() != TokenType::RIGHT_BRACE) {
    states.push_back(parseStmt());
  }
  consume(TokenType::RIGHT_BRACE, "Expected '}' at the end of block.");
  return std::make_unique<Block>(std::move(states));
}

Stmt Parser::parseStmt() {
    if (match({TokenType::FUNC}))
      return parseFunctionDeclStmt();
    if (match({TokenType::VAR}))
      return parseIdentifierDeclStmt();
    if (match({TokenType::IDENTIFIER})){
      if(tokens[pos+1].getType()==TokenType::EQUAL){
        return parseAssignStmt();
      }else{
        return parseExpressionStmt();
      }
    }

    if (match({TokenType::RETURN}))
      return parseReturnStmt();
    // Default to expression statement
    error("Unexpected token: " + peek().to_string());
}
Stmt Parser::parseFunctionDeclStmt() {
  advance(); // consume the 'func' token
  Token nameToken =
      consume(TokenType::IDENTIFIER, "Expected function name after 'func'.");
  std::string funcName = nameToken.getLexeme();
  consume(TokenType::LEFT_PAREN, "Expected '(' after function name.");
  std::vector<std::string> parameters;
  if (!match({TokenType::RIGHT_PAREN})) {
    do {
      if (match({TokenType::COMMA}))
        advance(); // consume the ',' token
      Token paramToken =
          consume(TokenType::IDENTIFIER, "Expected parameter name.");
      parameters.push_back(paramToken.getLexeme());
    } while (match({TokenType::COMMA}) );
  }
  consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters.");
  auto body = parseBlock();
  return std::make_unique<FunctionDeclStmt>(funcName, std::move(parameters), std::move(body));
}

Stmt Parser::parseIdentifierDeclStmt() {
  advance(); // consume the 'var' token
  Token nameToken =
      consume(TokenType::IDENTIFIER, "Expected identifier name after 'var'.");
  std::string identName = nameToken.getLexeme();
  if (match({TokenType::EQUAL})) {
    advance(); // consume the '=' token
    Expr value = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after identifier declaration.");
    return std::make_unique<IdentifierDeclStmt>(identName, std::move(value));
  } else {
    consume(TokenType::SEMICOLON, "Expected ';' after identifier declaration.");
    return std::make_unique<IdentifierDeclStmt>(identName);
  }
}

Stmt Parser::parseAssignStmt() {
  Token nameToken =
      consume(TokenType::IDENTIFIER, "Expected identifier name.");
  std::string identName = nameToken.getLexeme();
  consume(TokenType::EQUAL, "Expected '=' in assignment.");
  Expr value = parseExpression();
  consume(TokenType::SEMICOLON, "Expected ';' after assignment.");
  return std::make_unique<AssignStmt>(identName, std::move(value));
}
Stmt Parser::parseReturnStmt() {
  advance(); // consume the 'return' token
  Expr value = parseExpression();
  consume(TokenType::SEMICOLON, "Expected ';' after return statement.");
  return std::make_unique<ReturnStmt>(std::move(value));
}

Stmt Parser::parseExpressionStmt() {
  Expr expr = parseExpression();
  consume(TokenType::SEMICOLON, "Expected ';' after expression.");
  return std::make_unique<ExpressionStmt>(std::move(expr));
}

Expr Parser::parseExpression() {
  return parseTerm();
}

Expr Parser::parseTerm() {
  Expr left = parseFactor();
  while (match({TokenType::PLUS, TokenType::MINUS})) {
    Token opToken = advance();
    Binary::Op op = (opToken.getType() == TokenType::PLUS)
                        ? Binary::Op::ADD
                        : Binary::Op::SUB;
    Expr right = parseFactor();
    left = std::make_unique<Binary>(std::move(left), op, std::move(right));
  }
  return left;
}

Expr Parser::parseFactor() {
  Expr left = parsePrimary();
  while (match({TokenType::STAR, TokenType::SLASH})) {
    Token opToken = advance();
    Binary::Op op = (opToken.getType() == TokenType::STAR)
                        ? Binary::Op::MUL
                        : Binary::Op::DIV;
    Expr right = parsePrimary();
    left = std::make_unique<Binary>(std::move(left), op, std::move(right));
  }
  return left;
}

Expr Parser::parsePrimary() {
  if (match({TokenType::DOUBLE})) {
    return parseNumber();
  }
  if (match({TokenType::IDENTIFIER})) {
    if (tokens[pos + 1].getType() == TokenType::LEFT_PAREN) {
      return parseFunctionCallExpr();
    } else {
      return parseIdentifier();
    }
  }
  if (match({TokenType::LEFT_PAREN})) {
    return parseParenthesizedExpr();
  }
  error("Expected expression, found: " + peek().to_string());
}
Expr Parser::parseNumber() {
  Token numberToken = advance();
  double value = numberToken.getLiteral().value();
  return std::make_unique<Number>(value);
}

Expr Parser::parseFunctionCallExpr() {
  Token nameToken =
      consume(TokenType::IDENTIFIER, "Expected function name.");
  std::string funcName = nameToken.getLexeme();
  consume(TokenType::LEFT_PAREN, "Expected '(' after function name.");
  std::vector<Expr> arguments;
  if (!match({TokenType::RIGHT_PAREN})) {
    do {
      if (match({TokenType::COMMA}))
        advance(); // consume the ',' token
      Expr arg = parseExpression();
      arguments.push_back(std::move(arg));
    } while (match({TokenType::COMMA}) );
  }
  consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments.");
  return std::make_unique<FunctionCall>(funcName, std::move(arguments));
}

Expr Parser::parseIdentifier() {
  Token nameToken =
      consume(TokenType::IDENTIFIER, "Expected identifier name.");
  std::string identName = nameToken.getLexeme();
  return std::make_unique<Identifier>(identName);
}

Expr Parser::parseParenthesizedExpr() {
  consume(TokenType::LEFT_PAREN, "Expected '('.");
  Expr expr = parseExpression();
  consume(TokenType::RIGHT_PAREN, "Expected ')'.");
  return expr;
}

// int Parser::GetOpPrecedence(Binary::Op op) { return BinOpPrecedence.at(op){}
// }

} // namespace itry