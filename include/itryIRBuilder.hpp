#pragma once
#include "ast.hpp"
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
        builder(std::make_unique<::llvm::IRBuilder<>>(*context)) {
    module = std::make_unique<::llvm::Module>("itry_module", *context);
  }

public:


private:


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