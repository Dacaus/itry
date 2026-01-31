#include "itryType.hpp"

namespace itry {

struct GetTypeInfoVisitor {
  ItryTypeInfo operator()(std::monostate) const {
    throw std::runtime_error("Invalid type");
  }

  ItryTypeInfo operator()(std::shared_ptr<ItryBaseTypeInfo> baseType) const {
    return ItryTypeInfo(baseType);
  }

  ItryTypeInfo
  operator()(std::shared_ptr<ItryStructTypeInfo> structType) const {
    return ItryTypeInfo(structType);
  }
};

ItryTypeInfo getTypeInfo(std::string_view typeName) {
  ItryTypeManager &typeManager = ItryTypeManager::getTypeManager();
  auto typeInfoOpt = typeManager.getTypeInfo(std::string(typeName));
  if (typeInfoOpt.has_value()) {
    return typeInfoOpt.value();
  }
  throw std::runtime_error(std::string("Unknown type: ") + std::string(typeName));
}


struct GetTypeKindVistor {
  BaseTypeKind operator()(std::monostate) const {
    throw std::runtime_error("Invalid type");
  }

  BaseTypeKind operator()(std::shared_ptr<ItryBaseTypeInfo> baseType) const {
    return baseType->getBaseTypeKind();
  }

  BaseTypeKind
  operator()(std::shared_ptr<ItryStructTypeInfo> structType) const {
    return BaseTypeKind::CUSTOM;
  }
};

BaseTypeKind getTypeKind(const ItryTypeInfo &typeInfo) {
  return std::visit(GetTypeKindVistor{}, typeInfo);
}

struct GetTypeNameVisitor {
  std::string operator()(std::monostate) const {
    throw std::runtime_error("Invalid type");
  }

  std::string operator()(std::shared_ptr<ItryBaseTypeInfo> baseType) const {
    return baseType->getName();
  }

  std::string operator()(std::shared_ptr<ItryStructTypeInfo> structType) const {
    return structType->getName();
  }
};

std::string getTypeName(const ItryTypeInfo &typeInfo) {
  return std::visit(GetTypeNameVisitor{}, typeInfo);
}

struct GetTypeSizeVisitor {
  size_t operator()(std::monostate) const {
    throw std::runtime_error("Invalid type");
  }

  size_t operator()(std::shared_ptr<ItryBaseTypeInfo> baseType) const {
    return baseType->getSize();
  }

  size_t operator()(std::shared_ptr<ItryStructTypeInfo> structType) const {
    return structType->getSize();
  }
};

size_t getTypeSize(const ItryTypeInfo &typeInfo) {
  return std::visit(GetTypeSizeVisitor{}, typeInfo);
}




} // namespace itry