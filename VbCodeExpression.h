#pragma once
#include "VbCodeValue.h"
#include <memory>
#include <stdexcept>

class VbCodeExpression
{
public:
	virtual ~VbCodeExpression()
	{
	}

	virtual VbCodeValue EvaluateConstant() const
	{
		throw std::runtime_error("Cannot evaluate constant.");
	}
};

using VbCodeExpressionPtr = std::shared_ptr<VbCodeExpression>;
