#include "llvm/IR/Value.h"
#include <llvm-21/llvm/IR/Function.h>
#include <memory>
#include "ast.hpp"
#include "itryIRBuilder.hpp"
namespace itry {
class IRCodegen{
  public:
  llvm::Value* operator()(const Expr & expr,ItryIRBuilder& itryBuilder);
  
};
}