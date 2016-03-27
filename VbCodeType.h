#pragma once
#include "VbCodeValueType.h"
#include "VbCodeValue.h"
#include "VbCodeTypeName.h"
#include "optional.h"
#include <iostream>

class VbCodeType
{
public:
	VbCodeType() = default;
	VbCodeType(VbCodeValueType type)
		: type(type)
	{
	}
	VbCodeType(bool isNew, VbCodeValueType type, VbCodeTypeName userObject)
		: isNew(isNew), type(type), userObject(userObject)
	{
		if (type != VbCodeValueType::UserObject)
			throw std::runtime_error("Expected userObject!");
	}
	VbCodeType(VbCodeValueType type, VbCodeValue constantLength)
		: type(type), constantLength(constantLength)
	{
		if (type != VbCodeValueType::String)
			throw std::runtime_error("Expected string!");
	}
	VbCodeType(VbCodeValueType type, VbCodeTypeName variableLength)
		: type(type), variableLength(variableLength)
	{
		if (type != VbCodeValueType::String)
			throw std::runtime_error("Expected string!");
	}

	void WriteCs(std::ostream& out) const
	{
		if (isNew)
			throw std::runtime_error("As New not yet implemented");
		if (constantLength)
			throw std::runtime_error("Constant length string not yet supported.");
		if (variableLength)
			throw std::runtime_error("Variable length string not yet supported.");
		switch (type)
		{
		case VbCodeValueType::Boolean:
			out << "bool";
			break;
		case VbCodeValueType::Byte:
			out << "byte";
			break;
		case VbCodeValueType::Integer:
			out << "short";
			break;
		case VbCodeValueType::Long:
			out << "int";
			break;
		case VbCodeValueType::Single:
			out << "float";
			break;
		case VbCodeValueType::Double:
			out << "double";
			break;
		case VbCodeValueType::String:
			out << "string";
			break;
		case VbCodeValueType::Date:
			out << "DateTime";
			break;
		case VbCodeValueType::Currency:
			out << "decimal";
			break;
		case VbCodeValueType::Object:
			out << "object";
			break;
		case VbCodeValueType::Variant:
			out << "object";
			break;
		case VbCodeValueType::UserObject:
			if (userObject.library.empty())
				throw std::runtime_error("TODO: Need to resolve unqualified type names.");
			out << userObject.library << "." << userObject.name;
			break;
		default:
			throw std::runtime_error("Type not yet supported for WriteCs");
		}
	}

	bool isNew = false;
	VbCodeValueType type;
	VbCodeTypeName userObject;
	optional<VbCodeValue> constantLength;
	optional<VbCodeTypeName> variableLength;
};
