#pragma once
#include "VbCodeCaseType.h"
#include "VbCodeExpression.h"

class VbCodeCaseExpression
{
public:
	VbCodeCaseExpression(VbCodeExpressionPtr expression)
		: type(VbCodeCaseType::EqualTo), expression1(expression), expression2(nullptr)
	{
	}
	VbCodeCaseExpression(
		VbCodeCaseType type,
		VbCodeExpressionPtr expression)
		: type(type), expression1(expression), expression2(nullptr)
	{
	}
	VbCodeCaseExpression(
		VbCodeExpressionPtr expression1,
		VbCodeExpressionPtr expression2)
		: type(VbCodeCaseType::Between), expression1(expression1), expression2(expression2)
	{
	}

	VbCodeCaseType type;
	VbCodeExpressionPtr expression1;
	VbCodeExpressionPtr expression2;
};
