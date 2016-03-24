#pragma once
#include "VbCodeExpression.h"
#include "VbCodeBinaryExpressionType.h"

class VbCodeBinaryExpression : public VbCodeExpression
{
public:
	VbCodeBinaryExpression(
		VbCodeBinaryExpressionType type,
		VbCodeExpressionPtr lhs,
		VbCodeExpressionPtr rhs)
		: type(type), lhs(lhs), rhs(rhs)
	{
	}

public:
	VbCodeBinaryExpressionType type;
	VbCodeExpressionPtr lhs;
	VbCodeExpressionPtr rhs;
};
