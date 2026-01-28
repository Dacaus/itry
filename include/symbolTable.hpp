#pragma once

#include <llvm-21/llvm/IR/Value.h>
#include <memory>
#include <optional>
#include <unordered_map>
namespace itry {
class SymbolTable {
  std::shared_ptr<SymbolTable> parent;
  std::unordered_map<std::string, llvm::Value *> table;

public:
  SymbolTable() : parent(nullptr) {}
  SymbolTable(SymbolTable *parent) : parent(parent) {}

  void set(const std::string &name, llvm::Value *value) {
    auto tmpTable = this;
    while (tmpTable) {
      auto it = tmpTable->table.find(name);
      if (it != tmpTable->table.end()) {
        it->second = value;
        return;
      }
      tmpTable = tmpTable->parent.get();
    }
    table[name] = value;
  }

  std::optional<llvm::Value *> get(const std::string &name) {
    auto table = this;
    while (table) {
      auto it = table->table.find(name);
      if (it != table->table.end()) {
        return it->second;
      }
      table = table->parent.get();
    }
    return std::nullopt;
  }
};
}; // namespace itry