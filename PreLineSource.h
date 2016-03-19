#pragma once
#include "PreLine.h"
#include "PreToken.h"
#include <vector>

class PreLineSource : public PreLine
{
public:
	PreLineSource(const std::vector<PreToken>& tokens)
		: tokens(tokens)
	{
	}

	const std::vector<PreToken>& GetTokens() const
	{
		return tokens;
	}

	void Evaluate(PreLineEvaluator& evaluator) final;

private:
	std::vector<PreToken> tokens;
};
