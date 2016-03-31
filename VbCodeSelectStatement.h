#pragma once
#include "VbCodeStatement.h"
#include "VbCodeExpression.h"
#include "VbCodeCaseBlock.h"
#include "optional.h"
#include <vector>

class VbCodeSelectStatement : public VbCodeStatement
{
public:
	VbCodeSelectStatement(VbCodeExpressionPtr expression)
		: expression(expression)
	{
	}

	bool MatchesEnd(VbCodeEndType end) const final
	{
		return end == VbCodeEndType::Select;
	}
	std::vector<VbCodeStatementPtr>* Else() final
	{
		elseBlock = VbCodeCaseBlock{};
		return &elseBlock->statements;
	}
	std::vector<VbCodeStatementPtr>* Case(const std::vector<VbCodeCaseExpression>& expressions) final
	{
		caseBlocks.emplace_back(expressions);
		return &caseBlocks.back().statements;
	}

	void WriteCs(VbCodeWriter& writer) const final
	{
		//TODO:
		std::cout << "TODO: Select.WriteCs" << std::endl;
	}

	VbCodeExpressionPtr expression;
	std::vector<VbCodeCaseBlock> caseBlocks;
	optional<VbCodeCaseBlock> elseBlock;
};
