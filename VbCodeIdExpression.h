#pragma once
#include "VbCodeExpression.h"
#include <string>

class VbCodeIdExpression : public VbCodeExpression
{
public:
	VbCodeIdExpression(const std::string& name)
		: name(name)
	{
	}

	std::string name;
};
