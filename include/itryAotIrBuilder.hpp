#pragma once
#include "ast.hpp"
#include "symbolTable.hpp"
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <memory>
namespace itry {

class ItryIRBuilder {

public:
  ItryIRBuilder()
      : context(std::make_unique<llvm::LLVMContext>()),
        builder(std::make_unique<::llvm::IRBuilder<>>(*context)),
        symbolTable(std::make_unique<SymbolTable>()) {
    module = std::make_unique<::llvm::Module>("itry_module", *context);
    llvm::FunctionType *funcType =
        llvm::FunctionType::get(llvm::Type::getVoidTy(getContext()), false);
    llvm::Function *globalFunc = llvm::Function::Create(
        funcType, llvm::Function::ExternalLinkage, "__global__", &getModule());

    globalBlock = llvm::BasicBlock::Create(getContext(), "global", globalFunc);
    getBuilder().SetInsertPoint(globalBlock);
  }

  llvm::Module *generateCode(const std::vector<Stmt> &statements) {
    for (const auto &stmt : statements) {
      std::visit([this](auto &s) { this->generateCode(*s); }, stmt);
    }
    return module.get();
  }
  void printIR() { module->print(llvm::outs(), nullptr); }

private:
  llvm::Value *generateCode(Block &block) {
    llvm::Value *lastValue = nullptr;
    for (const auto &stmt : block.stmts) {
      lastValue =
          std::visit([this](auto &s) { return this->generateCode(*s); }, stmt);
    }
    return lastValue;
  }

  llvm::Value *generateCode(Number &number) {
    return llvm::ConstantFP::get(getContext(), llvm::APFloat(number.value));
  }

  llvm::Value *generateCode(Identifier &identifier) {
    // Lookup the variable in the symbol table
    auto value = symbolTable->get(identifier.name);
    if (value.has_value()) {
      return value.value();
    }
    throw std::runtime_error("Undefined variable: " + identifier.name);
  }

  llvm::Value *generateCode(Binary &binary) {
    llvm::Value *left = std::visit(
        [this](auto &expr) { return this->generateCode(*expr); }, binary.left);
    llvm::Value *right = std::visit(
        [this](auto &expr) { return this->generateCode(*expr); }, binary.right);

    switch (binary.op) {
    case Binary::Op::ADD:
      return getBuilder().CreateFAdd(left, right, "addtmp");
    case Binary::Op::SUB:
      return getBuilder().CreateFSub(left, right, "subtmp");
    case Binary::Op::MUL:
      return getBuilder().CreateFMul(left, right, "multmp");
    case Binary::Op::DIV:
      return getBuilder().CreateFDiv(left, right, "divtmp");
    default:
      throw std::runtime_error("Unknown binary operator");
    }
  }

  llvm::Value *generateCode(FunctionCall &funcCall) {
    llvm::Function *calleeFunc = getModule().getFunction(funcCall.name);
    if (!calleeFunc) {
      throw std::runtime_error("Unknown function referenced: " + funcCall.name);
    }

    std::vector<llvm::Value *> argsV;
    for (auto &arg : funcCall.args) {
      llvm::Value *argV = std::visit(
          [this](auto &expr) { return this->generateCode(*expr); }, arg);
      argsV.push_back(argV);
    }

    return getBuilder().CreateCall(calleeFunc, argsV, "calltmp");
  }

  
  llvm::Value *generateCode(AssignStmt &assignStmt) {
    llvm::Value *value =
        std::visit([this](auto &expr) { return this->generateCode(*expr); },
                   assignStmt.value);

    symbolTable->set(assignStmt.name, value);
    return value;
  }

  llvm::Value *generateCode(IdentifierDeclStmt &identDeclStmt) {
    llvm::Value *value = nullptr;
    if (identDeclStmt.value.has_value()) {
      value =
          std::visit([this](auto &expr) { return this->generateCode(*expr); },
                     identDeclStmt.value.value());
    } else {
      value = llvm::ConstantFP::get(getContext(), llvm::APFloat(0.0));
    }

    symbolTable->set(identDeclStmt.name, value);
    return value;
  }

  llvm::Value *generateCode(FunctionDeclStmt &funcDeclStmt) {
    std::vector<llvm::Type *> paramTypes(funcDeclStmt.params.size(),
                                         llvm::Type::getDoubleTy(getContext()));
    llvm::FunctionType *funcType = llvm::FunctionType::get(
        llvm::Type::getDoubleTy(getContext()), paramTypes, false);

    llvm::Function *function =
        llvm::Function::Create(funcType, llvm::Function::ExternalLinkage,
                               funcDeclStmt.name, &getModule());

    unsigned idx = 0;
    for (auto &arg : function->args()) {
      arg.setName(funcDeclStmt.params[idx++]);
      symbolTable->set(arg.getName().str(), &arg);
    }

    llvm::BasicBlock *bb =
        llvm::BasicBlock::Create(getContext(), "entry", function);
    getBuilder().SetInsertPoint(bb);

    generateCode(*funcDeclStmt.body);

    if (getBuilder().GetInsertBlock()->getTerminator() == nullptr) {
      getBuilder().CreateRet(
          llvm::ConstantFP::get(getContext(), llvm::APFloat(0.0)));
    }
    getBuilder().SetInsertPoint(globalBlock);
    return function;
  }

  llvm::Value *generateCode(ExpressionStmt &exprStmt) {
    return std::visit([this](auto &expr) { return this->generateCode(*expr); },
                      exprStmt.expr);
  }

  llvm::Value *generateCode(ReturnStmt &returnStmt) {
    llvm::Value *retValue =
        std::visit([this](auto &expr) { return this->generateCode(*expr); },
                   returnStmt.value);
    return getBuilder().CreateRet(retValue);
  }

private:
  llvm::LLVMContext &getContext() { return *context; }
  llvm::IRBuilder<> &getBuilder() { return *builder; }
  llvm::Module &getModule() { return *module; }

private:
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<llvm::Module> module;
  std::unique_ptr<SymbolTable> symbolTable;
  llvm::BasicBlock *globalBlock;
};

} // namespace itry