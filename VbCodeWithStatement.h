#pragma once
#include "VbCodeStatement.h"
#include <vector>

class VbCodeWithStatement : public VbCodeStatement
{
public:

	bool MatchesEnd(VbCodeEndType end) const final
	{
		return end == VbCodeEndType::With;
	}

	//TODO: l-value
	std::vector<VbCodeStatementPtr> statements;
};
