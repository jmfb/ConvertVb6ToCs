#pragma once
#include "VbCodeEndType.h"
#include <memory>

class VbCodeStatement
{
public:
	virtual ~VbCodeStatement()
	{
	}

	void MatchEnd(VbCodeEndType end) const
	{
		if (!MatchesEnd(end))
			throw std::runtime_error("Mismatched end.");
	}
	virtual bool MatchesEnd(VbCodeEndType end) const
	{
		return false;
	}
};

using VbCodeStatementPtr = std::shared_ptr<VbCodeStatement>;
