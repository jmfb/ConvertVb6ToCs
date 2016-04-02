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
