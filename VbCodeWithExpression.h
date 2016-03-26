#pragma once
#include "VbCodeExpression.h"
#include "VbCodeDotType.h"
#include <string>

class VbCodeWithExpression : public VbCodeExpression
{
public:
	VbCodeWithExpression(VbCodeDotType dot, const std::string& member)
		: dot(dot), member(member)
	{
	}

	VbCodeDotType dot;
	std::string member;
};
