#pragma once
#include <stdexcept>

enum class VbCodeValueType
{
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
	UserObject
};

inline const char* ToString(VbCodeValueType value)
{
	switch (value)
	{
	case VbCodeValueType::Boolean: return "Boolean";
	case VbCodeValueType::Byte: return "Byte";
	case VbCodeValueType::Integer: return "Integer";
	case VbCodeValueType::Long: return "Long";
	case VbCodeValueType::Single: return "Single";
	case VbCodeValueType::Double: return "Double";
	case VbCodeValueType::Currency: return "Currency";
	case VbCodeValueType::Date: return "Date";
	case VbCodeValueType::String: return "String";
	case VbCodeValueType::Variant: return "Variant";
	case VbCodeValueType::Object: return "Object";
	case VbCodeValueType::UserObject: return "UserObject";
	}
	throw std::runtime_error("Invalid value type.");
}
