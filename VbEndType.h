#pragma once
#include <stdexcept>

enum class VbEndType
{
	If,
	Select,
	With,
	Program,
	Sub,
	Function,
	Property
};

const char* ToString(VbEndType value)
{
	switch (value)
	{
	case VbEndType::If: return "End If";
	case VbEndType::Select: return "End Select";
	case VbEndType::With: return "End With";
	case VbEndType::Program: return "End";
	case VbEndType::Sub: return "End Sub";
	case VbEndType::Function: return "End Function";
	case VbEndType::Property: return "End Property";
	}
	throw std::runtime_error("Invalid EndType");
}
