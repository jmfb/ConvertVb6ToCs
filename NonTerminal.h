#pragma once
#include "Production.h"
#include "Terminal.h"
#include <string>
#include <vector>
#include <set>

class NonTerminal
{
public:
	NonTerminal(const std::string& name = "")
		: name(name)
	{
	}

	const std::string& GetName() const
	{
		return name;
	}
	const std::vector<Production>& GetItems() const
	{
		return items;
	}
	std::vector<Production>& GetItems()
	{
		return items;
	}
	const std::set<Terminal>& GetFirst() const
	{
		return first;
	}
	const std::set<Terminal>& GetFollow() const
	{
		return follow;
	}
	std::set<Terminal>& GetFollow()
	{
		return follow;
	}

	bool IsEmpty() const
	{
		return items.empty();
	}

	void AddItem(const Production& production)
	{
		items.push_back(production);
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

	void AddFollow(const Terminal& terminal)
	{
		follow.insert(terminal);
	}
	void MergeFollow(const std::set<Terminal>& source)
	{
		follow.insert(source.begin(), source.end());
	}

	void AddFirstToFollow(const std::set<Terminal>& source)
	{
		std::copy_if(
			source.begin(),
			source.end(),
			std::inserter(follow, follow.begin()),
			[](auto& t){ return t != Terminal::EndOfFile(); });
	}

public:
	std::string name;
	std::vector<Production> items;
	std::set<Terminal> first;
	std::set<Terminal> follow;
};
