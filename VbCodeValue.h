#pragma once
#include "VbCodeValueType.h"
#include "String.h"
#include <string>
#include <iostream>

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

	void WriteXml(std::ostream& out) const
	{
		out << ToString(type) << " = ";
		switch (type)
		{
		case VbCodeValueType::Boolean:
			out << (boolValue ? "True" : "False");
			break;
		case VbCodeValueType::Byte:
			out << static_cast<unsigned int>(byteValue);
			break;
		case VbCodeValueType::Integer:
			out << integerValue;
			break;
		case VbCodeValueType::Long:
			out << longValue;
			break;
		case VbCodeValueType::Single:
			out << singleValue;
			break;
		case VbCodeValueType::Double:
			out << doubleValue;
			break;
		case VbCodeValueType::String:
			out << "\"" << String::EscapeXml(stringValue) << "\"";
			break;
		default:
			throw std::runtime_error("Value type not yet supported for WriteXml");
		}
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
