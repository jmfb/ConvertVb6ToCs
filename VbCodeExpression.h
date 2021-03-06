#pragma once
#include "VbCodeValue.h"
#include "VbCodeWriter.h"
#include <memory>
#include <stdexcept>
#include <iostream>

class VbCodeExpression
{
public:
	virtual ~VbCodeExpression()
	{
	}

	virtual void WriteCs(VbCodeWriter& writer) const = 0;
	virtual VbCodeValue EvaluateConstant() const
	{
		throw std::runtime_error("Cannot evaluate constant.");
	}
};

using VbCodeExpressionPtr = std::shared_ptr<VbCodeExpression>;
