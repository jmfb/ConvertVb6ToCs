#pragma once
#include "VbCodeExpression.h"
#include <string>

class VbCodeEnumMember
{
public:
	VbCodeEnumMember(const std::string& name, VbCodeExpressionPtr expression)
		: name(name), expression(expression)
	{
	}

	std::string name;
	VbCodeExpressionPtr expression;
};
