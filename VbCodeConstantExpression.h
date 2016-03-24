#pragma once
#include "VbCodeExpression.h"
#include "VbCodeValue.h"

class VbCodeConstantExpression : public VbCodeExpression
{
public:
	VbCodeConstantExpression(VbCodeValue value)
		: value(value)
	{
	}

	VbCodeValue EvaluateConstant() const final
	{
		return value;
	}

public:
	VbCodeValue value;
};
