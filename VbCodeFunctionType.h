#pragma once
#include <stdexcept>

enum class VbCodeFunctionType
{
	Sub,
	Function,
	PropertyGet,
	PropertyLet,
	PropertySet
};

inline const char* ToString(VbCodeFunctionType value)
{
	switch (value)
	{
	case VbCodeFunctionType::Sub: return "Sub";
	case VbCodeFunctionType::Function: return "Function";
	case VbCodeFunctionType::PropertyGet: return "Property Get";
	case VbCodeFunctionType::PropertyLet: return "Property Let";
	case VbCodeFunctionType::PropertySet: return "Property Set";
	}
	throw std::runtime_error("Invalid function type.");
}
