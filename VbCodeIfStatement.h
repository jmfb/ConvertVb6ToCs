#pragma once
#include "VbCodeStatement.h"
#include "VbCodeIfBlock.h"
#include "optional.h"

class VbCodeIfStatement : public VbCodeStatement
{
public:
	VbCodeIfStatement(VbCodeExpressionPtr expression)
	{
		ifBlocks.emplace_back(expression);
	}

	bool MatchesEnd(VbCodeEndType end) const final
	{
		return end == VbCodeEndType::If;
	}
	std::vector<VbCodeStatementPtr>* ElseIf(VbCodeExpressionPtr expression) final
	{
		if (elseBlock)
			throw std::runtime_error("ElseIf cannot follow an Else");
		ifBlocks.emplace_back(expression);
		return &ifBlocks.back().statements;
	}
	std::vector<VbCodeStatementPtr>* Else() final
	{
		if (elseBlock)
			throw std::runtime_error("Only one else block is supported per If.");
		elseBlock = std::vector<VbCodeStatementPtr>{};
		return &*elseBlock;
	}

	std::vector<VbCodeIfBlock> ifBlocks;
	optional<std::vector<VbCodeStatementPtr>> elseBlock;
};
