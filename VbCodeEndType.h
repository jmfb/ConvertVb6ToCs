#pragma once
#include <stdexcept>

enum class VbCodeEndType
{
	If,
	Select,
	With,
	Program,
	Sub,
	Function,
	Property
};

const char* ToString(VbCodeEndType value)
{
	switch (value)
	{
	case VbCodeEndType::If: return "End If";
	case VbCodeEndType::Select: return "End Select";
	case VbCodeEndType::With: return "End With";
	case VbCodeEndType::Program: return "End";
	case VbCodeEndType::Sub: return "End Sub";
	case VbCodeEndType::Function: return "End Function";
	case VbCodeEndType::Property: return "End Property";
	}
	throw std::runtime_error("Invalid end type");
}