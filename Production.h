#pragma once
#include "ProductionItem.h"
#include "Terminal.h"
#include <deque>
#include <set>
#include <algorithm>
#include <iterator>

class Production
{
public:
	const std::deque<ProductionItem>& GetItems() const
	{
		return items;
	}
	const std::set<Terminal>& GetFirst() const
	{
		return first;
	}

	Production& AddItem(const Terminal& terminal)
	{
		items.emplace_back(terminal);
		return *this;
	}
	Production& AddItem(const std::string& nonTerminal)
	{
		items.emplace_back(nonTerminal);
		return *this;
	}
	Production& AddItem(const ProductionItem& item)
	{
		items.push_back(item);
		return *this;
	}

	void RemoveFront()
	{
		if (items.empty())
			throw std::runtime_error("Cannot remove the front of an empty production.");
		items.pop_front();
	}
	
	void AddFirst(const Terminal& terminal)
	{
		first.insert(terminal);
	}

	void MergeFirst(const std::set<Terminal>& source)
	{
		std::copy_if(
			source.begin(),
			source.end(),
			std::inserter(first, first.begin()),
			[](auto& t){ return t != Terminal::EndOfFile(); });
	}

	bool IsEmpty() const
	{
		return items.empty();
	}

	bool operator==(const Production& rhs) const
	{
		return items == rhs.items;
	}

	bool operator!=(const Production& rhs) const
	{
		return !operator==(rhs);
	}

public:
	std::deque<ProductionItem> items;
	std::set<Terminal> first;
};
