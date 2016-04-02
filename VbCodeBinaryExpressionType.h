#pragma once
#include "EnumParser.h"

enum class VbCodeBinaryExpressionType
{
	Multiplication,
	Division,
	IntegerDivision,
	Modulus,
	Exponent,
	Addition,
	Subtraction,
	StringConcatenation,
	GreaterThan,
	GreaterThanOrEqual,
	LessThan,
	LessThanOrEqual,
	EqualTo,
	NotEqualTo,
	Implies,
	Equivalent,
	Is,
	Like,
	And,
	Xor,
	Or
};

inline VbCodeBinaryExpressionType ToEqualityOperator(const Token& token)
{
	return EnumParser::Parse<VbCodeBinaryExpressionType>(
		token,
		{
			{ "=", VbCodeBinaryExpressionType::EqualTo },
			{ "<>", VbCodeBinaryExpressionType::NotEqualTo },
			{ "imp", VbCodeBinaryExpressionType::Implies },
			{ "eqv", VbCodeBinaryExpressionType::Equivalent },
			{ "is", VbCodeBinaryExpressionType::Is },
			{ "like", VbCodeBinaryExpressionType::Like }
		});
}

inline VbCodeBinaryExpressionType ToRelationalOperator(const Token& token)
{
	return EnumParser::Parse<VbCodeBinaryExpressionType>(
		token,
		{
			{ ">=", VbCodeBinaryExpressionType::GreaterThanOrEqual },
			{ ">", VbCodeBinaryExpressionType::GreaterThan },
			{ "<=", VbCodeBinaryExpressionType::LessThanOrEqual },
			{ "<", VbCodeBinaryExpressionType::LessThan }
		});
}

inline VbCodeBinaryExpressionType ToAdditiveOperator(const Token& token)
{
	return EnumParser::Parse<VbCodeBinaryExpressionType>(
		token,
		{
			{ "+", VbCodeBinaryExpressionType::Addition },
			{ "-", VbCodeBinaryExpressionType::Subtraction },
			{ "&", VbCodeBinaryExpressionType::StringConcatenation }
		});
}

inline VbCodeBinaryExpressionType ToMultiplicativeOperator(const Token& token)
{
	return EnumParser::Parse<VbCodeBinaryExpressionType>(
		token,
		{
			{ "*", VbCodeBinaryExpressionType::Multiplication },
			{ "/", VbCodeBinaryExpressionType::Division },
			{ "\\", VbCodeBinaryExpressionType::IntegerDivision },
			{ "mod", VbCodeBinaryExpressionType::Modulus },
			{ "^", VbCodeBinaryExpressionType::Exponent }
		});
}
