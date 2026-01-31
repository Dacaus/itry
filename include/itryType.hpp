

#pragma once
#include <llvm/IR/DataLayout.h>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>
namespace itry {
enum class BaseTypeKind {
  INT,    // 整数类型
  DOUBLE, // 浮点数类型
  BOOL,   // 布尔类型
  VOID,   // 空类型
  CUSTOM  // 自定义类型（用于结构体）
};

class ItryBaseTypeInfo;
class ItryStructTypeInfo;

// 使用shared_ptr来打破循环依赖问题
using ItryTypeInfo =
    std::variant<std::monostate, std::shared_ptr<ItryBaseTypeInfo>,
                 std::shared_ptr<ItryStructTypeInfo>>;

class ItryBaseTypeInfo {
public:
  ItryBaseTypeInfo(BaseTypeKind kind, const std::string &name, size_t size)
      : kind(kind), typeName(name), typeSize(size) {}

  std::string getName() const { return typeName; }
  size_t getSize() const { return typeSize; }
  BaseTypeKind getBaseTypeKind() const { return kind; }

private:
  BaseTypeKind kind;
  std::string typeName;
  size_t typeSize;
};

struct StructMember {
  std::string name;      // 成员名称
  ItryTypeInfo typeInfo; // 成员类型
  size_t offset;         // 成员在结构体中的偏移量（字节）

  StructMember(const std::string &name, ItryTypeInfo type, size_t offset = 0)
      : name(name), typeInfo(type), offset(offset) {}
};

class ItryStructTypeInfo {
public:
  ItryStructTypeInfo(const std::string &name, std::vector<StructMember> mems)
      : structName(name), members(std::move(mems)) {
    // 更新成员的偏移量
    size_t offset = 0;
    for (auto &member : members) {
      member.offset = offset;
      // Here we assume all types are of size 8 for simplicity
      offset += 8;
    }
    totalSize = offset;
  }

  std::string getName() const { return structName; }
  size_t getSize() const { return totalSize; }
  const std::vector<StructMember> &getMembers() const { return members; }

private:
  std::string structName;
  std::vector<StructMember> members;
  size_t totalSize;
};

class ItryTypeManager {
public:
  static ItryTypeManager &getTypeManager() {
    static ItryTypeManager instance;
    return instance;
  }

  std::optional<ItryTypeInfo> getTypeInfo(const std::string &typeName) {
    auto it = types.find(typeName);
    if (it != types.end()) {
      return it->second;
    }
    return std::nullopt;
  }

  bool registerStructType(const ItryStructTypeInfo &structType) {
    const auto &name = structType.getName();
    if (types.find(name) != types.end()) {
      return false; // 结构体已存在
    }
    types[name] = ItryTypeInfo(
        std::make_shared<ItryStructTypeInfo>(structType));
    return true;
  }


private:
  ItryTypeManager() {
    // Initialize basic types
    types["int"] = ItryTypeInfo(
        std::make_shared<ItryBaseTypeInfo>(BaseTypeKind::INT, "int", 4));
    types["double"] = ItryTypeInfo(
        std::make_shared<ItryBaseTypeInfo>(BaseTypeKind::DOUBLE, "double", 8));
    types["bool"] = ItryTypeInfo(
        std::make_shared<ItryBaseTypeInfo>(BaseTypeKind::BOOL, "bool", 1));
    types["void"] = ItryTypeInfo(
        std::make_shared<ItryBaseTypeInfo>(BaseTypeKind::VOID, "void", 0));
  }
  std::map<std::string, ItryTypeInfo> types;
};

} // namespace itry