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

	void WriteXml(std::ostream& out) const final
	{
		out << "<if-statement>";
		ifBlocks[0].WriteXml(out);
		out << "</if-statement>";
		for (auto index = 1u; index < ifBlocks.size(); ++index)
		{
			out << "<elseif-statement>";
			ifBlocks[index].WriteXml(out);
			out << "</elseif-statement>";
		}
		if (elseBlock)
		{
			out << "<else-statement>";
			for (auto& statement : *elseBlock)
				statement->WriteXml(out);
			out << "</else-statement>";
		}
	}

	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		writer.StartLine();
		writer.out << "if (";
		ifBlocks[0].expression->WriteCs(writer); //TODO: determine if conversion to bool is required
		writer.out << ")" << std::endl;
		writer.BeginBlock();
		for (auto& statement : ifBlocks[0].statements)
			statement->WriteCs(writer);
		writer.EndBlock();
		for (auto index = 1u; index < ifBlocks.size(); ++index)
		{
			writer.StartLine();
			writer.out << "else if (";
			ifBlocks[index].expression->WriteCs(writer); //TODO: same
			writer.out << ")" << std::endl;
			writer.BeginBlock();
			for (auto& statement : ifBlocks[index].statements)
				statement->WriteCs(writer);
			writer.EndBlock();
		}
		if (!elseBlock)
			return;
		writer.StartLine();
		writer.out << "else" << std::endl;
		writer.BeginBlock();
		for (auto& statement : *elseBlock)
			statement->WriteCs(writer);
		writer.EndBlock();
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
