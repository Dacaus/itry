#pragma once
#include "ast.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <mutex>
#include <random>
namespace itry {

class ItryIRBuilder {
public:
  ItryIRBuilder()
      : context(std::make_unique<llvm::LLVMContext>()),
        builder(std::make_unique<::llvm::IRBuilder<>>(*context)) {
    module = std::make_unique<::llvm::Module>("itry_module", *context);
  }


  llvm::Value* doubleCodegen(double num)  {
    return llvm::ConstantFP::get(getContext(), llvm::APFloat(num));
  }

  llvm::Value* CreateFAdd(llvm::Value* L, llvm::Value* R, const std::string& name = "addtmp") {
    return getBuilder().CreateFAdd(L, R, name);
  }

  llvm::Value* CreateFSub(llvm::Value* L, llvm::Value* R, const std::string& name = "subtmp") {
    return getBuilder().CreateFSub(L, R, name);
  }

  llvm::Value* CreateFMul(llvm::Value* L, llvm::Value* R, const std::string& name = "multmp") {
    return getBuilder().CreateFMul(L, R, name);
  }

  llvm::Value* CreateFDiv(llvm::Value* L, llvm::Value* R, const std::string& name = "divtmp") {
    return getBuilder().CreateFDiv(L, R, name);
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