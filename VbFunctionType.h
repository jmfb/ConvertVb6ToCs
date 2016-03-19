#pragma once
#include <stdexcept>

enum class VbFunctionType
{
	Sub,
	Function,
	PropertyGet,
	PropertyLet,
	PropertySet
};

inline const char* ToString(VbFunctionType value)
{
	switch (value)
	{
	case VbFunctionType::Sub: return "Sub";
	case VbFunctionType::Function: return "Function";
	case VbFunctionType::PropertyGet: return "Property Get";
	case VbFunctionType::PropertyLet: return "Property Let";
	case VbFunctionType::PropertySet: return "Property Set";
	}
	throw std::runtime_error("Invalid function type.");
}
