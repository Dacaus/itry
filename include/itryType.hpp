// #pragma once
// #include "ast.hpp"
// #include <memory>
// #include <optional>
// #include <string>
// #include <unordered_map>
// #include <variant>
// #include <vector>
// #include <memory>
// namespace itry {

// //
// ============================================================================
// // 类型系统设计说明
// //
// ============================================================================
// // 设计目标：
// // 1. 支持基本类型（int, double, bool等）
// // 2. 支持结构体类型（可嵌套，包含多个成员）
// // 3. 统一的类型表示和查询接口
// // 4. 类型的动态注册和检索

// //
// ============================================================================
// // 基本类型枚举
// //
// ============================================================================
// enum class BaseTypeKind {
//   INT,      // 整数类型
//   DOUBLE,   // 浮点数类型
//   BOOL,     // 布尔类型
//   VOID,     // 空类型
//   CUSTOM    // 自定义类型（用于结构体）
// };

// //
// ============================================================================
// // 类型信息的基类 - 使用多态支持不同类型
// //
// ============================================================================
// class ITypeInfo {
// public:
//   virtual ~ITypeInfo() = default;

//   // 获取类型的字符串表示
//   virtual std::string getName() const = 0;

//   // 获取类型的大小（以字节计）
//   virtual size_t getSize() const = 0;

//   // 判断是否为基本类型
//   virtual bool isBaseType() const = 0;

//   // 判断是否为结构体类型
//   virtual bool isStructType() const = 0;

//   // 获取基本类型种类（若是基本类型）
//   virtual BaseTypeKind getBaseTypeKind() const {
//     return BaseTypeKind::VOID;
//   }
// };

// //
// ============================================================================
// // 基本类型信息
// //
// ============================================================================
// class BasicTypeInfo : public ITypeInfo {
// private:
//   BaseTypeKind kind;
//   std::string typeName;
//   size_t typeSize;

//   // 私有构造函数
//   BasicTypeInfo(BaseTypeKind kind, const std::string &name, size_t size)
//       : kind(kind), typeName(name), typeSize(size) {}

// public:
//   // 工厂方法 - 创建预定义的基本类型
//   static std::shared_ptr<BasicTypeInfo> createInt() {
//     return std::make_shared<BasicTypeInfo>(BaseTypeKind::INT, "int", 4);
//   }

//   static std::shared_ptr<BasicTypeInfo> createDouble() {
//     return std::make_shared<BasicTypeInfo>(BaseTypeKind::DOUBLE, "double",
//     8);
//   }

//   static std::shared_ptr<BasicTypeInfo> createBool() {
//     return std::make_shared<BasicTypeInfo>(BaseTypeKind::BOOL, "bool", 1);
//   }

//   static std::shared_ptr<BasicTypeInfo> createVoid() {
//     return std::make_shared<BasicTypeInfo>(BaseTypeKind::VOID, "void", 0);
//   }

//   // 实现接口方法
//   std::string getName() const override { return typeName; }
//   size_t getSize() const override { return typeSize; }
//   bool isBaseType() const override { return true; }
//   bool isStructType() const override { return false; }
//   BaseTypeKind getBaseTypeKind() const override { return kind; }
// };

// //
// ============================================================================
// // 结构体成员信息
// //
// ============================================================================
// struct StructMember {
//   std::string name;                    // 成员名称
//   std::shared_ptr<ITypeInfo> typeInfo; // 成员类型
//   size_t offset;                       // 成员在结构体中的偏移量（字节）

//   StructMember(const std::string &name, std::shared_ptr<ITypeInfo> type,
//                size_t offset = 0)
//       : name(name), typeInfo(type), offset(offset) {}
// };

// //
// ============================================================================
// // 结构体类型信息
// //
// ============================================================================
// class StructTypeInfo : public ITypeInfo {
// private:
//   std::string structName;
//   std::vector<StructMember> members;
//   size_t totalSize;

//   // 计算结构体总大小
//   size_t calculateSize() const {
//     size_t size = 0;
//     for (const auto &member : members) {
//       size += member.typeInfo->getSize();
//     }
//     return size;
//   }

// public:
//   // 构造函数
//   StructTypeInfo(const std::string &name, std::vector<StructMember> mems)
//       : structName(name), members(std::move(mems)) {
//     totalSize = calculateSize();
//     // 更新成员的偏移量
//     size_t offset = 0;
//     for (auto &member : members) {
//       member.offset = offset;
//       offset += member.typeInfo->getSize();
//     }
//   }

// // 实现接口方法
//   std::string getName() const override { return structName; }
//   size_t getSize() const override { return totalSize; }
//   bool isBaseType() const override { return false; }
//   bool isStructType() const override { return true; }

//   // 结构体特定的方法
//   const std::vector<StructMember> &getMembers() const { return members; }

//   // 通过成员名称查找成员信息
//   std::optional<const StructMember *> getMemberByName(
//       const std::string &memberName) const {
//     for (const auto &member : members) {
//       if (member.name == memberName) {
//         return &member;
//       }
//     }
//     return std::nullopt;
//   }

//   // 通过索引获取成员
//   std::optional<const StructMember *> getMemberByIndex(size_t index) const {
//     if (index < members.size()) {
//       return &members[index];
//     }
//     return std::nullopt;
//   }

//   // 获取成员数量
//   size_t getMemberCount() const { return members.size(); }
// };

// //
// ============================================================================
// // 类型管理器 - 全局的类型注册表
// //
// ============================================================================
// class TypeManager {
// private:
//   // 基本类型缓存
//   std::unordered_map<std::string, std::shared_ptr<BasicTypeInfo>>
//       basicTypes;

//   // 结构体类型存储
//   std::unordered_map<std::string, std::shared_ptr<StructTypeInfo>>
//       structTypes;

//   // 私有构造函数（单例模式）
//   TypeManager() { initializeBuiltinTypes(); }

//   // 初始化内置的基本类型
//   void initializeBuiltinTypes() {
//     basicTypes["int"] = BasicTypeInfo::createInt();
//     basicTypes["double"] = BasicTypeInfo::createDouble();
//     basicTypes["bool"] = BasicTypeInfo::createBool();
//     basicTypes["void"] = BasicTypeInfo::createVoid();
//   }

// public:
//   // 获取类型管理器的单例实例
//   static TypeManager &getInstance() {
//     static TypeManager instance;
//     return instance;
//   }

//   // 防止拷贝
//   TypeManager(const TypeManager &) = delete;
//   TypeManager &operator=(const TypeManager &) = delete;

//   // ========== 基本类型查询 ==========

//   // 获取基本类型
//   std::shared_ptr<BasicTypeInfo> getBasicType(const std::string &typeName) {
//     auto it = basicTypes.find(typeName);
//     if (it != basicTypes.end()) {
//       return it->second;
//     }
//     return nullptr;
//   }

//   // ========== 结构体类型管理 ==========

//   // 注册新的结构体类型
//   bool registerStructType(std::shared_ptr<StructTypeInfo> structType) {
//     const auto &name = structType->getName();
//     if (structTypes.find(name) != structTypes.end()) {
//       return false; // 结构体已存在
//     }
//     structTypes[name] = structType;
//     return true;
//   }

//   // 获取结构体类型
//   std::shared_ptr<StructTypeInfo> getStructType(const std::string &typeName)
//   {
//     auto it = structTypes.find(typeName);
//     if (it != structTypes.end()) {
//       return it->second;
//     }
//     return nullptr;
//   }

//   // 检查类型是否存在
//   bool typeExists(const std::string &typeName) const {
//     return basicTypes.find(typeName) != basicTypes.end() ||
//            structTypes.find(typeName) != structTypes.end();
//   }

//   // 获取任意类型（基本类型或结构体）
//   std::shared_ptr<ITypeInfo> getType(const std::string &typeName) {
//     if (auto basicType = getBasicType(typeName)) {
//       return basicType;
//     }
//     return getStructType(typeName);
//   }

//   // 列出所有已注册的结构体类型
//   std::vector<std::string> getAllStructTypeNames() const {
//     std::vector<std::string> names;
//     for (const auto &pair : structTypes) {
//       names.push_back(pair.first);
//     }
//     return names;
//   }
// };

// } // namespace itry

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

// class ItryTypeManager {
//   public:
//     static ItryTypeManager &getInstance() {
//       static ItryTypeManager instance;
//       return instance;
//     }

//     ItryTypeManager(const ItryTypeManager &) = delete;
//     ItryTypeManager &operator=(const ItryTypeManager &) = delete;

//     std::shared_ptr<ItryBaseTypeInfo> getBasicType(const std::string
//     &typeName) {
//       auto it = basicTypes.find(typeName);
//       if (it != basicTypes.end()) {
//         return it->second;
//       }
//       return nullptr;
//     }

//     bool registerStructType(std::shared_ptr<ItryStructTypeInfo> structType) {
//       const auto &name = structType->getName();
//       if (structTypes.find(name) != structTypes.end()) {
//         return false; // 结构体已存在
//       }
//       structTypes[name] = structType;
//       return true;
//     }

//     std::shared_ptr<ItryStructTypeInfo> getStructType(const std::string
//     &typeName) {
//       auto it = structTypes.find(typeName);
//       if (it != structTypes.end()) {
//         return it->second;
//       }
//       return nullptr;
//     }

//   private:
//     ItryTypeManager() { initializeBuiltinTypes(); }

//     void initializeBuiltinTypes() {
//       basicTypes["int"] =
//       std::make_shared<ItryBaseTypeInfo>(BaseTypeKind::INT, "int", 4);
//       basicTypes["double"] =
//       std::make_shared<ItryBaseTypeInfo>(BaseTypeKind::DOUBLE, "double", 8);
//       basicTypes["bool"] =
//       std::make_shared<ItryBaseTypeInfo>(BaseTypeKind::BOOL, "bool", 1);
//       basicTypes["void"] =
//       std::make_shared<ItryBaseTypeInfo>(BaseTypeKind::VOID, "void", 0);
//     }

//     std::unordered_map<std::string, std::shared_ptr<ItryBaseTypeInfo>>
//     basicTypes; std::unordered_map<std::string,
//     std::shared_ptr<ItryStructTypeInfo>> structTypes;
// };

} // namespace itry