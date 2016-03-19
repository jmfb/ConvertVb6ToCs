#pragma once
#include "PreLine.h"
#include "PreIfBlock.h"
#include <list>

class PreLineIf : public PreLine
{
public:
	PreLineIf(const PreIfBlock& firstIf)
	{
		blocks.push_back(firstIf);
	}

	const std::list<PreIfBlock>& GetBlocks() const
	{
		return blocks;
	}
	std::list<PreIfBlock>& GetBlocks()
	{
		return blocks;
	}

	void Evaluate(PreLineEvaluator& evaluator) final;

private:
	std::list<PreIfBlock> blocks;
};
