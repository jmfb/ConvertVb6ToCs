#pragma once
#include "NonTerminal.h"
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include <stdexcept>

class Grammar
{
public:
	Grammar(const std::map<std::string, NonTerminal> items)
		: items(items)
	{
	}

	const std::map<std::string, NonTerminal>& GetItems() const
	{
		return items;
	}

	void AddItem(const NonTerminal& nonTerminal)
	{
		items[nonTerminal.GetName()] = nonTerminal;
	}

	void Augment(const std::string& start, const std::string& augmentedStart)
	{
		Production production;
		production.AddItem(start);
		NonTerminal nonTerminal{ augmentedStart };
		nonTerminal.AddItem(production);
		AddItem(nonTerminal);
	}

	void CalculateFirst()
	{
		for (auto& item : items)
			if (item.second.GetFirst().empty())
				CalculateFirst(item.second, 1);
	}

	void CalculateFollow(const std::string& start)
	{
		//Add $ (input right end marker) to start symbol
		auto iter = items.find(start);
		if (iter == items.end())
			throw std::runtime_error("Start symbol was not found in grammar.");
		iter->second.GetFollow().insert(Terminal::EndOfFile());

		//If exists A->aBb, then FOLLOW(B) += FIRST(b) - @
		std::vector<std::pair<std::string, std::string>> merge;
		for (auto& pair : items)
		{
			auto& nonTerminal = pair.second;
			for (auto& production : nonTerminal.GetItems())
			{
				if (production.IsEmpty())
					continue;
				for (auto index = 0u; index < production.GetItems().size(); ++index)
				{
					auto& item = production.GetItems()[index];
					if (item.IsNonTerminal())
					{
						iter = items.find(item.GetNonTerminal());
						if (iter == items.end())
							throw std::runtime_error("Undefined non-terminal");
						auto& current = iter->second;
						auto emptyCount = 0u;
						for (auto next = index + 1; next < production.GetItems().size(); ++next)
						{
							auto& after = production.GetItems()[next];
							if (after.IsTerminal())
							{
								current.AddFollow(after.GetTerminal());
								break;
							}
							else
							{
								iter = items.find(after.GetNonTerminal());
								if (iter == items.end())
									throw std::runtime_error("Undefined non-terminal");
								auto& lookup = items[after.GetNonTerminal()];
								current.AddFirstToFollow(lookup.GetFirst());
								if (lookup.GetFirst().find(Terminal::EndOfFile()) != lookup.GetFirst().end())
									++emptyCount;
								else
									break;
							}
						}
						if (emptyCount == (production.GetItems().size() - index - 1))
							merge.emplace_back(current.GetName(), nonTerminal.GetName());
					}
				}
			}
		}

		//If exists A->aB or A->aBb where FIRST(b) has @, then FOLLOW(B) += FOLLOW(A)
		while (!merge.empty())
			ResolveFollow(merge.front().first, merge);
	}

private:
	void CalculateFirst(NonTerminal& nonTerminal, int depth)
	{
		if (depth == 256)
			throw std::runtime_error("Recursive non-terminal detected.");
		for (auto& production : nonTerminal.GetItems())
		{
			if (production.IsEmpty())
				nonTerminal.AddFirst(Terminal::EndOfFile());
			else if (production.GetItems().front().IsTerminal())
			{
				auto& terminal = production.GetItems().front().GetTerminal();
				production.AddFirst(terminal);
				nonTerminal.AddFirst(terminal);
			}
			else
			{
				auto emptyCount = 0u;
				for (auto& item : production.GetItems())
				{
					if (item.IsTerminal())
					{
						production.AddFirst(item.GetTerminal());
						nonTerminal.AddFirst(item.GetTerminal());
						break;
					}

					auto iter = items.find(item.GetNonTerminal());
					if (iter == items.end())
						throw std::runtime_error("Undefined non-terminal referenced.");
					auto& lookup = iter->second;
					if (lookup.GetFirst().empty())
						CalculateFirst(lookup, depth + 1);
					production.MergeFirst(lookup.GetFirst());
					nonTerminal.MergeFirst(lookup.GetFirst());
					if (lookup.GetFirst().find(Terminal::EndOfFile()) != lookup.GetFirst().end())
						++emptyCount;
					else
						break;
				}
				if (emptyCount == production.GetItems().size())
				{
					production.AddFirst(Terminal::EndOfFile());
					nonTerminal.AddFirst(Terminal::EndOfFile());
				}
			}
		}
	}

	void ResolveFollow(const std::string& name, std::vector<std::pair<std::string, std::string>>& merge)
	{
		std::vector<std::string> sources;
		for (auto index = 0u; index < merge.size(); )
		{
			auto& pair = merge[index];
			if (pair.first == name && std::find(sources.begin(), sources.end(), pair.second) == sources.end())
			{
				sources.push_back(pair.second);
				merge.erase(merge.begin() + index);
			}
			else
				++index;
		}
		for (auto index = 0u; index < sources.size(); ++index)
			for (auto& pair : merge)
				if (pair.first == sources[index] && pair.second != name && std::find(sources.begin(), sources.end(), pair.second) == sources.end())
					sources.push_back(pair.second);
		for (auto& source : sources)
			items[name].MergeFollow(items[source].GetFollow());
	}

public:
	std::map<std::string, NonTerminal> items;
};
