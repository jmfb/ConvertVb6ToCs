#pragma once
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
		for (auto& type : types)
			if (type.second.scope == 0 &&
				type.second.name == name &&
				type.second.category == TypeCategory::Library)
				return type.first;
		throw std::runtime_error("Library not defined: " + name);
	}

private:
	TypeHandle DefineType(
		TypeHandle scope,
		const std::string& name,
		TypeCategory category)
	{
		for (auto& type : types)
			if (type.second.scope == scope && type.second.name == name)
				if (type.second.category == category)
					throw std::runtime_error("Redefinition: " + name);
				else
					throw std::runtime_error("Redefinition (different type): " + name);
		auto handle = nextHandle++;
		types[handle] = { scope, name, category, handle };
		return handle;
	}

private:
	TypeHandle nextHandle = 10;
	std::map<TypeHandle, TypeData> types;
};
