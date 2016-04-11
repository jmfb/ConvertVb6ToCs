#pragma once
#include "VbCodeType.h"
#include "optional.h"
#include <string>
#include <map>
#include <vector>

using TypeHandle = int;

enum class TypeCategory
{
	Void,
	Boolean,
	Byte,
	Integer,
	Long,
	Single,
	Double,
	Currency,
	Date,
	String,
	Variant,
	Object,
	Array,
	UserClass,
	UserEnum,
	UserEnumValue,
	Function,
	Library
};

inline const char* ToString(TypeCategory value)
{
	switch (value)
	{
	case TypeCategory::Void: return "void";
	case TypeCategory::Boolean: return "boolean";
	case TypeCategory::Byte: return "byte";
	case TypeCategory::Integer: return "integer";
	case TypeCategory::Long: return "long";
	case TypeCategory::Single: return "single";
	case TypeCategory::Double: return "double";
	case TypeCategory::Currency: return "currency";
	case TypeCategory::Date: return "date";
	case TypeCategory::String: return "string";
	case TypeCategory::Variant: return "variant";
	case TypeCategory::Object: return "object";
	case TypeCategory::Array: return "array";
	case TypeCategory::UserClass: return "class";
	case TypeCategory::UserEnum: return "enum";
	case TypeCategory::UserEnumValue: return "enumvalue";
	case TypeCategory::Function: return "function";
	case TypeCategory::Library: return "library";
	}
	throw std::runtime_error("Invalid type category.");
}

static const constexpr TypeHandle TypeHandleVoid = 0;
static const constexpr TypeHandle TypeHandleBoolean = 1;
static const constexpr TypeHandle TypeHandleByte = 2;
static const constexpr TypeHandle TypeHandleInteger = 3;
static const constexpr TypeHandle TypeHandleLong = 4;
static const constexpr TypeHandle TypeHandleSingle = 5;
static const constexpr TypeHandle TypeHandleDouble = 6;
static const constexpr TypeHandle TypeHandleCurrency = 7;
static const constexpr TypeHandle TypeHandleDate = 8;
static const constexpr TypeHandle TypeHandleString = 9;
static const constexpr TypeHandle TypeHandleVariant = 10;
static const constexpr TypeHandle TypeHandleObject = 11;

class TypeArgument
{
public:

private:
	bool byRef;
	std::string name;
	TypeHandle type;
};

class TypeData
{
public:
	TypeData() = default;
	TypeData(
		TypeHandle scope,
		const std::string& name,
		TypeCategory category,
		TypeHandle handle)
		: scope(scope), name(name), category(category), handle(handle)
	{
	}

	TypeHandle scope = 0;
	std::string name;
	TypeCategory category = TypeCategory::Void;
	TypeHandle handle = 0;
	TypeHandle resultType = 0;
	std::map<std::string, TypeHandle> members;
	std::vector<TypeArgument> arguments;
};

class TypeTable
{
public:
	TypeHandle DefineLibrary(const std::string& name)
	{
		return DefineType(0, name, TypeCategory::Library);
	}

	TypeHandle DefineClass(TypeHandle scope, const std::string& name)
	{
		return DefineType(scope, name, TypeCategory::UserClass);
	}

	void DefineMember(TypeHandle parent, const std::string& memberName, TypeHandle memberType)
	{
		auto& type = types[parent];
		if (type.category != TypeCategory::UserClass)
			throw std::runtime_error("Can only add members to classes.");
		if (type.members.find(memberName) != type.members.end())
			throw std::runtime_error("Redefinition of member: " + memberName);
		types[parent].members[memberName] = memberType;
	}

	TypeHandle DefineEnum(TypeHandle scope, const std::string& name)
	{
		return DefineType(scope, name, TypeCategory::UserEnum);
	}

	TypeHandle DefineEnumValue(TypeHandle enumHandle, const std::string& name)
	{
		auto handle = DefineType(enumHandle, name, TypeCategory::UserEnumValue);
		types[handle].resultType = enumHandle;
		return handle;
	}

	TypeHandle GetLibrary(const std::string& name) const
	{
		auto type = TryGetType(0, name);
		if (!type)
			throw std::runtime_error("Library not defined: " + name);
		if (type->category != TypeCategory::Library)
			throw std::runtime_error("WTF? Top level element not a library.");
		return type->handle;
	}

	TypeHandle GetClass(TypeHandle scope, const std::string& name) const
	{
		auto type = TryGetType(scope, name);
		if (!type)
			throw std::runtime_error("Class not defined: " + name);
		if (type->category != TypeCategory::UserClass)
			throw std::runtime_error("Type was not a class: " + name);
		return type->handle;
	}

	TypeHandle ResolveType(TypeHandle scope, const VbCodeType& codeType) const
	{
		switch (codeType.type)
		{
		case VbCodeValueType::Boolean:
			return TypeHandleBoolean;
		case VbCodeValueType::Byte:
			return TypeHandleByte;
		case VbCodeValueType::Integer:
			return TypeHandleInteger;
		case VbCodeValueType::Long:
			return TypeHandleLong;
		case VbCodeValueType::Single:
			return TypeHandleSingle;
		case VbCodeValueType::Double:
			return TypeHandleDouble;
		case VbCodeValueType::Currency:
			return TypeHandleCurrency;
		case VbCodeValueType::Date:
			return TypeHandleDate;
		case VbCodeValueType::String:
			return TypeHandleString;
		case VbCodeValueType::Variant:
			return TypeHandleVariant;
		case VbCodeValueType::Object:
			return TypeHandleObject;
		case VbCodeValueType::UserObject:
			return ResolveNamedType(scope, codeType.userObject);
		}
		throw std::runtime_error("Invalid type to resolve.");
	}

	TypeHandle ResolveNamedType(TypeHandle scope, const VbCodeTypeName& typeName) const
	{
		if (!typeName.library.empty())
			return GetClass(GetLibrary(typeName.library), typeName.name);
		for (auto parentScope = scope; parentScope != 0; parentScope = types.find(parentScope)->second.scope)
		{
			auto type = TryGetType(parentScope, typeName.name);
			if (type)
				return type->handle;
		}
		throw std::runtime_error("Type not defined: " + typeName.name);
	}

	void Dump()
	{
		for (auto& type : types)
		{
			std::cout << type.first << ": "
				<< "scope(" << type.second.scope << ") "
				<< ToString(type.second.category) << " "
				<< type.second.name << std::endl;
			for (auto& member : type.second.members)
				std::cout << "  " << member.first << " = " << member.second << std::endl;
		}
	}

private:
	TypeHandle DefineType(
		TypeHandle scope,
		const std::string& name,
		TypeCategory category)
	{
		if (FindType(scope, name))
			throw std::runtime_error("Redefinition: " + name);
		auto handle = nextHandle++;
		types[handle] = { scope, name, category, handle };
		typesByScope[scope][name] = handle;
		return handle;
	}

	optional<TypeData> TryGetType(TypeHandle scope, const std::string& name) const
	{
		auto handle = FindType(scope, name);
		if (handle)
			return GetType(*handle);
		return{};
	}

	const TypeData& GetType(TypeHandle handle) const
	{
		auto iter = types.find(handle);
		if (iter == types.end())
			throw std::runtime_error("Type not found.");
		return iter->second;
	}

	optional<TypeHandle> FindType(TypeHandle scope, const std::string& name) const
	{
		auto scopeTypes = typesByScope.find(scope);
		if (scopeTypes == typesByScope.end())
			return{};
		auto type = scopeTypes->second.find(name);
		if (type == scopeTypes->second.end())
			return{};
		return type->second;
	}

private:
	TypeHandle nextHandle = 100;
	std::map<TypeHandle, TypeData> types;
	std::map<TypeHandle, std::map<std::string, TypeHandle>> typesByScope;
};
