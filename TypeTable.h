#pragma once
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
	Function,
	Library
};

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

	TypeHandle DefineEnum(TypeHandle scope, const std::string& name)
	{
		return DefineType(scope, name, TypeCategory::UserEnum);
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
	TypeHandle nextHandle = 10;
	std::map<TypeHandle, TypeData> types;
	std::map<TypeHandle, std::map<std::string, TypeHandle>> typesByScope;
};
