#pragma once
#include "VbCodeValueType.h"
#include "VbCodeValue.h"
#include "VbCodeTypeName.h"
#include "optional.h"

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

	bool isNew = false;
	VbCodeValueType type;
	VbCodeTypeName userObject;
	optional<VbCodeValue> constantLength;
	optional<VbCodeTypeName> variableLength;
};
