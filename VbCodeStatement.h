#pragma once
#include "VbCodeEndType.h"
#include "VbCodeExpression.h"
#include <memory>
#include <vector>
#include <iostream>

class VbCodeStatement;
using VbCodeStatementPtr = std::shared_ptr<VbCodeStatement>;

class VbCodeStatement
{
public:
	virtual ~VbCodeStatement()
	{
	}

	virtual void WriteXml(std::ostream& out) const = 0;

	void MatchEnd(VbCodeEndType end) const
	{
		if (!MatchesEnd(end))
			throw std::runtime_error("Mismatched end.");
	}
	virtual bool MatchesEnd(VbCodeEndType end) const
	{
		return false;
	}
	virtual std::vector<VbCodeStatementPtr>* ElseIf(VbCodeExpressionPtr expression)
	{
		throw std::runtime_error("ElseIf could not find matching If");
	}
	virtual std::vector<VbCodeStatementPtr>* Else()
	{
		throw std::runtime_error("Else could not find matching If");
	}
};

