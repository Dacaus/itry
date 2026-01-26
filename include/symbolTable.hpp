#pragma once

#include <llvm-21/llvm/IR/Value.h>
#include <optional>
#include <unordered_map>
namespace itry {
class SymbolTable{
  std::unordered_map<std::string, llvm::Value*> table;
  
public:
  void set(const std::string &name, llvm::Value* value){
    table[name] = value;
  }

  std::optional<llvm::Value*> get(const std::string &name){
    if(table.find(name) != table.end()){
      return table[name];
    }
    return std::nullopt;
  }
};
};