#pragma once
#include "Production.h"
#include "ProductionItem.h"
#include "Terminal.h"
#include <string>
#include <cstddef>
#include <sstream>

class Item1
{
public:
	Item1() = delete;
	Item1(
		const std::string& nonTerminal,
		const Production& production,
		std::size_t index,
		const Terminal& follow)
		: nonTerminal(nonTerminal), production(production), index(index), follow(follow)
	{
	}
	Item1(const Item1& rhs) = default;
	~Item1() = default;

	Item1& operator=(const Item1& rhs) = default;

	bool HasNonTerminalSuffix(std::string& suffix) const
	{
		// A -> a.Bb
		if (index >= production.items.size() || !production.items[index].isNonTerminal)
			return false;
		suffix = production.items[index].nonTerminal;
		return true;
	}
	bool IsNext(const ProductionItem& item) const
	{
		return index < production.items.size() && production.items[index] == item;
	}
	ProductionItem GetNext() const
	{
		if (index < production.items.size())
			return production.items[index];
		return{};
	}

	std::string ToString() const
	{
		std::ostringstream out;
		out << nonTerminal << " = ";
		for (auto index = 0ul; index < production.items.size(); ++index)
		{
			if (index == this->index)
				out << ". ";
			out << production.items[index].ToString() << " ";
		}
		if (index >= production.items.size())
			out << ". ";
		out << ", " << follow.ToString();
		return out.str();
	}

	bool operator==(const Item1& rhs) const
	{
		return nonTerminal == rhs.nonTerminal &&
			production == rhs.production &&
			index == rhs.index &&
			follow == rhs.follow;
	}
	bool operator!=(const Item1& rhs) const
	{
		return !operator==(rhs);
	}

public:
	std::string nonTerminal;
	Production production;
	std::size_t index;
	Terminal follow;
};
