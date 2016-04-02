#pragma once
#include "EnumParser.h"

enum class VbCodeCaseType
{
	EqualTo,
	NotEqualTo,
	GreaterThan,
	GreaterThanOrEqual,
	LessThan,
	LessThanOrEqual,
	Between
};

inline VbCodeCaseType ToCaseType(const Token& token)
{
	return EnumParser::Parse<VbCodeCaseType>(
		token,
		{
			{ ">=", VbCodeCaseType::GreaterThanOrEqual },
			{ ">", VbCodeCaseType::GreaterThan },
			{ "<=", VbCodeCaseType::LessThanOrEqual },
			{ "<", VbCodeCaseType::LessThan },
			{ "<>", VbCodeCaseType::NotEqualTo },
			{ "=", VbCodeCaseType::EqualTo }
		});
}

inline const char* ToCs(VbCodeCaseType value)
{
	switch (value)
	{
	case VbCodeCaseType::EqualTo: return "==";
	case VbCodeCaseType::NotEqualTo: return "!=";
	case VbCodeCaseType::GreaterThan: return ">";
	case VbCodeCaseType::GreaterThanOrEqual: return ">=";
	case VbCodeCaseType::LessThan: return "<";
	case VbCodeCaseType::LessThanOrEqual: return "<=";
	}
	throw std::runtime_error("Invalid case type operator for C#");
}
