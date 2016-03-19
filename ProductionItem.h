#pragma once
#include "Terminal.h"
#include <string>

class ProductionItem
{
public:
	ProductionItem() = default;
	ProductionItem(const Terminal& terminal)
		: isTerminal(true), terminal(terminal)
	{
	}
	ProductionItem(const std::string& nonTerminal)
		: isNonTerminal(true), terminal(Terminal::EndOfFile()), nonTerminal(nonTerminal)
	{
	}

	bool IsTerminal() const
	{
		return isTerminal;
	}
	bool IsNonTerminal() const
	{
		return isNonTerminal;
	}
	bool IsNull() const
	{
		return !isTerminal && !isNonTerminal;
	}

	const Terminal& GetTerminal() const
	{
		return terminal;
	}
	const std::string& GetNonTerminal() const
	{
		return nonTerminal;
	}

	bool operator==(const ProductionItem& rhs) const
	{
		if (IsNull() != rhs.IsNull())
			return false;
		if (IsNull())
			return true;
		if (IsTerminal() != rhs.IsTerminal())
			return false;
		return IsTerminal() ?
			terminal == rhs.terminal :
			nonTerminal == rhs.nonTerminal;
	}
	bool operator!=(const ProductionItem& rhs) const
	{
		return !operator==(rhs);
	}

	std::string ToString() const
	{
		if (IsNull())
			return "null";
		if (isTerminal)
			return terminal.ToString();
		std::ostringstream out;
		out << "<" << nonTerminal << ">";
		return out.str();
	}

public:
	bool isTerminal = false;
	bool isNonTerminal = false;
	Terminal terminal;
	std::string nonTerminal;
};
