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
		if (CanConvertToSwitch())
			WriteSwitch(writer);
		else
			WriteChainedIfElse(writer);
	}

	bool CanConvertToSwitch() const
	{
		//TODO: check type of select expression to make sure it is valid for switch statements.
		for (auto& caseBlock : caseBlocks)
			for (auto& caseExpression : caseBlock.expressions)
				if (caseExpression.type != VbCodeCaseType::EqualTo)
					return false;
		return true;
	}

	void WriteSwitch(VbCodeWriter& writer) const
	{
		writer.StartLine();
		writer.out << "switch (";
		expression->WriteCs(writer);
		writer.out << ")" << std::endl;
		writer.BeginBlock();
		writer.PopIndent();
		for (auto& caseBlock : caseBlocks)
		{
			for (auto& caseExpression : caseBlock.expressions)
			{
				writer.StartLine();
				writer.out << "case ";
				caseExpression.expression1->WriteCs(writer);
				writer.out << ":" << std::endl;
			}
			writer.PushIndent();
			for (auto& statement : caseBlock.statements)
				statement->WriteCs(writer);
			writer.StartLine();
			writer.out << "break;" << std::endl;
			writer.PopIndent();
		}
		if (elseBlock)
		{
			writer.StartLine();
			writer.out << "default:" << std::endl;
			writer.PushIndent();
			for (auto& statement : elseBlock->statements)
				statement->WriteCs(writer);
			writer.StartLine();
			writer.out << "break;" << std::endl;
			writer.PopIndent();
		}
		writer.PushIndent();
		writer.EndBlock();
	}

	void WriteChainedIfElse(VbCodeWriter& writer) const
	{
		writer.StartLine();
		auto select = writer.NextSelect();
		writer.out << "var __select" << select << " = ";
		expression->WriteCs(writer);
		writer.out << ";" << std::endl;
		if (!caseBlocks.empty())
		{
			writer.StartLine();
			writer.out << "if (";
			caseBlocks[0].WriteIfExpression(writer, select);
			writer.out << ")" << std::endl;
			writer.BeginBlock();
			for (auto& statement : caseBlocks[0].statements)
				statement->WriteCs(writer);
			writer.EndBlock();
			for (auto index = 1u; index < caseBlocks.size(); ++index)
			{
				writer.StartLine();
				writer.out << "else if (";
				caseBlocks[index].WriteIfExpression(writer, select);
				writer.out << ")" << std::endl;
				writer.BeginBlock();
				for (auto& statement : caseBlocks[0].statements)
					statement->WriteCs(writer);
				writer.EndBlock();
			}
		}
		if (!elseBlock)
			return;
		if (caseBlocks.empty())
		{
			for (auto& statement : elseBlock->statements)
				statement->WriteCs(writer);
		}
		else
		{
			writer.StartLine();
			writer.out << "else" << std::endl;
			writer.BeginBlock();
			for (auto& statement : elseBlock->statements)
				statement->WriteCs(writer);
			writer.EndBlock();
		}
	}

	VbCodeExpressionPtr expression;
	std::vector<VbCodeCaseBlock> caseBlocks;
	optional<VbCodeCaseBlock> elseBlock;
};
