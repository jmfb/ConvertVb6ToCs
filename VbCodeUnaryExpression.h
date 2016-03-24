#pragma once
#include "VbCodeExpression.h"
#include "VbCodeUnaryExpressionType.h"

class VbCodeUnaryExpression : public VbCodeExpression
{
public:
	VbCodeUnaryExpression(
		VbCodeUnaryExpressionType type,
		VbCodeExpressionPtr expression)
		: type(type), expression(expression)
	{
	}

public:
	VbCodeUnaryExpressionType type;
	VbCodeExpressionPtr expression;
};
