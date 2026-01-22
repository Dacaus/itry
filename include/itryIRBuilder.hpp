#pragma once
#include "ast.hpp"
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <memory>
namespace itry {

class ItryIRBuilder {
  
public:
  ItryIRBuilder()
      : context(std::make_unique<llvm::LLVMContext>()),
        builder(std::make_unique<::llvm::IRBuilder<>>(*context)) {
    module = std::make_unique<::llvm::Module>("itry_module", *context);
  }

public:

  llvm::Function *createMainFunction() {
    llvm::FunctionType *funcType =
        llvm::FunctionType::get(llvm::Type::getDoubleTy(*context), false);
    llvm::Function *mainFunc = llvm::Function::Create(
        funcType, llvm::Function::ExternalLinkage, "main", getModule());
    llvm::BasicBlock *entry =
        llvm::BasicBlock::Create(*context, "entry", mainFunc);
    getBuilder().SetInsertPoint(entry);
    return mainFunc;
  }

private:
  llvm::Value *exprCodegen(const Expr &expr) {
    if(std::holds_alternative<Number>(expr)){
      return numberCodegen(std::get<Number>(expr));
    }
    else if(std::holds_alternative<std::unique_ptr<Binary>>(expr)){
      return binaryOpCodegen(*std::get<std::unique_ptr<Binary>>(expr));
    }
    else if(std::holds_alternative<std::unique_ptr<Assignment>>(expr)){
      return assignmentCodegen(*std::get<std::unique_ptr<Assignment>>(expr));
    }
  }

  llvm::Value *binaryOpCodegen(const Binary &binary) {
    llvm::Value *left = exprCodegen(binary.lhs);
    llvm::Value *right = exprCodegen(binary.rhs);

    switch (binary.op) {
    case Binary::Op::Add:
      return getBuilder().CreateFAdd(left, right, "addtmp");
    case Binary::Op::Subtract:
      return getBuilder().CreateFSub(left, right, "subtmp");
    case Binary::Op::Multiply:
      return getBuilder().CreateFMul(left, right, "multmp");
    case Binary::Op::Divide:
      return getBuilder().CreateFDiv(left, right, "divtmp");
    default:
      return nullptr;
    }
  }

  llvm::Value *numberCodegen(Number num) {
    return llvm::ConstantFP::get(getContext(), llvm::APFloat(num.value));
  }

  llvm::Value *assignmentCodegen(const Assignment &assignment) {
    // 这里假设所有变量都是全局变量，且已经在模块中声明
    llvm::GlobalVariable *var = getModule().getGlobalVariable(assignment.identifier);
    if (!var) {
      // 如果变量不存在，则创建一个新的全局变量
      var = new llvm::GlobalVariable(
          getModule(), llvm::Type::getDoubleTy(*context), false,
          llvm::GlobalValue::ExternalLinkage,
          llvm::ConstantFP::get(getContext(), llvm::APFloat(0.0)),
          assignment.identifier);
    }
    llvm::Value *value = exprCodegen(assignment.value);
    getBuilder().CreateStore(value, var);
    return value;
  }


private:
  llvm::LLVMContext &getContext() { return *context; }
  llvm::IRBuilder<> &getBuilder() { return *builder; }
  llvm::Module &getModule() { return *module; }

private:
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<llvm::Module> module;
};

} // namespace itry