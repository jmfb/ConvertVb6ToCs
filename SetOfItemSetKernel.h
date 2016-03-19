#pragma once
#include "ItemSetX.h"
#include "ItemSet1.h"
#include "Grammar.h"
#include <vector>
#include <map>
#include <string>
#include <cstddef>
#include <algorithm>

class SetOfItemSetKernel
{
public:
	SetOfItemSetKernel() = delete;
	SetOfItemSetKernel(Grammar& g, const std::string& augmentedStart)
		: augmentedStart(augmentedStart), grammar(g)
	{
		//Seed data with [$$ -> . start]
		ItemSetX initial;
		initial.AddItem({ augmentedStart, g.items[augmentedStart].items[0], 0 });
		data.push_back(initial);

		//Construct set of LR(0) kernel items
		for (auto index = 0ul; index < data.size(); ++index)
		{
			std::map<std::string, std::size_t> gotoTable;
			ItemSetX items = data[index].Closure(g);
			auto nexts = items.CalculateUniqueNext();
			for (auto& pitem : nexts)
			{
				ItemSetX itemGoto = items.GotoRaw(pitem);
				if (!itemGoto.items.empty())
				{
					auto next = 0ul;
					auto iter = std::find(data.begin(), data.end(), itemGoto);
					if (iter == data.end())
					{
						next = data.size();
						data.push_back(itemGoto);
					}
					else
					{
						next = iter - data.begin();
					}
					gotoTable.insert({ pitem.ToString(), next });
				}
			}
			gotos.push_back(gotoTable);
		}

		//Spontaneously generate look aheads and generate propogation rules
		DetermineLookAheads();

		//Propogate until no more look aheads are propogated
		while (Propogate());
	}
	SetOfItemSetKernel(const SetOfItemSetKernel& rhs) = default;
	~SetOfItemSetKernel() = default;

	SetOfItemSetKernel& operator=(const SetOfItemSetKernel& rhs) = default;

private:
	bool Propogate()
	{
		auto retval = false;
		for (auto& itemset : data)
			for (auto& item : itemset.items)
				for (auto& prop : item.propogate)
					for (auto& terminal : item.lookAhead)
						retval |= data[prop.state].items[prop.subItem].AddLookAhead(terminal);
		return retval;
	}
	void DetermineLookAheads()
	{
		//Loop through each state I in K
		for (auto index = 0ul; index < data.size(); ++index)
		{
			//I{index} in K
			auto& itemset0 = data[index];

			//Loop through each kernel item in state I{index}.
			for (auto& item0 : itemset0.items)
			{
				//Special case [S' -> . S, $] is spontaneously generated for augmented start
				if (item0.nonTerminal == augmentedStart && item0.index == 0)
					item0.AddLookAhead(Terminal::EndOfFile());

				//Introduce a LookAhead terminal and calculate closure
				ItemSet1 itemset1;
				itemset1.AddItem({ item0.nonTerminal, item0.production, item0.index, Terminal::LookAhead() });
				auto closure = itemset1.Closure(grammar);

				//for each [A->s.Xt,?] in closure([B->q.r,#])
				for (auto& item1 : closure.items)
				{
					//Grammar symbol X in [A->s.Xt]
					auto grammar_symbol = item1.GetNext();
					if (!grammar_symbol.IsNull())
					{
						//Determine got(I,X) (index of [A->sX.t])
						auto next = gotos[index][grammar_symbol.ToString()];
						auto subitem = data[next].FindSubItem(item1.production, item1.index + 1);
						if (item1.follow == Terminal::LookAhead())
						{
							// [A->s.Xt,#] then propagate from this to [A->sX.t]
							item0.AddPropogate(next, subitem);
						}
						else
						{
							// [A->s.Xt,a] then [A->sX.t,a] is look ahead
							data[next].items[subitem].AddLookAhead(item1.follow);
						}
					}
				}
			}
		}
	}

public:
	std::vector<ItemSetX> data;
	std::vector<std::map<std::string, std::size_t>> gotos;
	std::string augmentedStart;
	Grammar& grammar;
};
