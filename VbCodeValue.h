#pragma once
#include "VbCodeValueType.h"
#include <string>

class VbCodeValue
{
public:
	VbCodeValue()
		: type(VbCodeValueType::Variant) //Null, Nothing, or Empty?
	{
	}
	VbCodeValue(bool value)
		: type(VbCodeValueType::Boolean), boolValue(value)
	{
	}
	VbCodeValue(std::uint8_t value)
		: type(VbCodeValueType::Byte), byteValue(value)
	{
	}
	VbCodeValue(short value)
		: type(VbCodeValueType::Integer), integerValue(value)
	{
	}
	VbCodeValue(int value)
		: type(VbCodeValueType::Long), longValue(value)
	{
	}
	VbCodeValue(float value)
		: type(VbCodeValueType::Single), singleValue(value)
	{
	}
	VbCodeValue(double value)
		: type(VbCodeValueType::Double), doubleValue(value)
	{
	}
	VbCodeValue(const std::string& value)
		: type(VbCodeValueType::String), stringValue(value)
	{
	}

public:
	VbCodeValueType type;
	bool boolValue;
	std::uint8_t byteValue;
	short integerValue;
	int longValue;
	float singleValue;
	double doubleValue;
	//currency?
	//date?
	std::string stringValue;
	//variant?
	//object?
	//userObject?
};
