#pragma once
#include "TransitionTable.h"
#include "ConflictResolver.h"
#include "Grammar.h"
#include "GrammarParser.h"
#include "SetOfItemSetKernel.h"
#include "TransitionList.h"
#include "ItemSet1.h"
#include <string>

class ParserLALR
{
public:
	ParserLALR() = delete;
	ParserLALR(const ParserLALR& rhs) = delete;
	~ParserLALR() = delete;

	ParserLALR& operator=(const ParserLALR& rhs) = delete;

	static TransitionTable Generate(const std::string& grammar, const std::string& start, ConflictResolver& resolver)
	{
		GrammarParser parser;
		std::cout << "Parsing grammar...";
		auto g = parser.Parse(grammar);
		std::cout << "done." << std::endl;

		std::cout << "Computing first/follow...";
		const auto augmentedStart = "$$";
		g.Augment(start, augmentedStart);
		g.CalculateFirst();
		g.CalculateFollow(augmentedStart);
		std::cout << "done." << std::endl;

		std::cout << "Calculating transition table...";
		SetOfItemSetKernel kernel{ g, augmentedStart };

		//Build LR(1) transition table;
		TransitionTable table;
		for (auto index = 0ul; index < kernel.data.size(); ++index)
		{
			auto& itemset0 = kernel.data[index];
			auto& gotos = kernel.gotos[index];
			TransitionList entry{ table.table.size() };

			//Expand LR(0) kernel items and look-aheads into closure LR(1) set
			ItemSet1 itemset1;
			for (auto& item0 : itemset0.items)
				for (auto& terminal : item0.lookAhead)
					itemset1.AddItem({ item0.nonTerminal, *item0.production, item0.index, terminal });
			itemset1 = itemset1.Closure(g);

			//Repeat normal LR(1) transition table construction
			for (auto& item : itemset1.items)
			{
				auto pitem = item.GetNext();
				if (pitem.IsNull())
				{
					if (item.nonTerminal == augmentedStart)
					{
						if (item.production->items.size() == 1 &&
							item.production->items[0].isNonTerminal &&
							item.production->items[0].nonTerminal == start)
							entry.AddAction(Terminal::EndOfFile(), {}, resolver);
					}
					else
					{
						entry.AddAction(item.follow, { item.nonTerminal, *item.production }, resolver);
					}
				}
				else
				{
					auto next = gotos[pitem.ToString()];
					if (pitem.isTerminal)
						entry.AddAction(pitem.terminal, { next }, resolver);
					else
						entry.AddGoto(pitem.nonTerminal, next);
				}
			}

			table.table.push_back(entry);
		}

		std::cout << "done." << std::endl;
		return table;
	}
};
