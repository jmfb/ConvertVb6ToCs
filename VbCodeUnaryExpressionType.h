#pragma once
#include "EnumParser.h"

enum class VbCodeUnaryExpressionType
{
	Not,
	Negate,
	FileNumber
};

inline VbCodeUnaryExpressionType ToUnaryOperator(const Token& token)
{
	return EnumParser::Parse<VbCodeUnaryExpressionType>(
		token,
		{
			{ "not", VbCodeUnaryExpressionType::Not },
			{ "-", VbCodeUnaryExpressionType::Negate },
			{ "#", VbCodeUnaryExpressionType::FileNumber }
		});
}
