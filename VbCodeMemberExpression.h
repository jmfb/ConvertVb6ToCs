#pragma once
#include "VbCodeExpression.h"
#include "VbCodeDotType.h"
#include <string>

class VbCodeMemberExpression : public VbCodeExpression
{
public:
	VbCodeMemberExpression(
		VbCodeExpressionPtr source,
		VbCodeDotType dot,
		const std::string& member)
		: source(source), dot(dot), member(member)
	{
	}

	VbCodeExpressionPtr source;
	VbCodeDotType dot;
	std::string member;
};
