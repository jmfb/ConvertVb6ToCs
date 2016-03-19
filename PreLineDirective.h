#pragma once
#include "PreLine.h"
#include "PreDirectiveType.h"
#include "Position.h"
#include "PreToken.h"
#include <vector>

class PreLineDirective : public PreLine
{
public:
	PreLineDirective(PreDirectiveType type, const Position& position, const std::vector<PreToken>& tokens)
		: type(type), position(position), tokens(tokens)
	{
	}

	PreDirectiveType GetType() const
	{
		return type;
	}
	const Position& GetPosition() const
	{
		return position;
	}
	const std::vector<PreToken>& GetTokens() const
	{
		return tokens;
	}

	void Evaluate(PreLineEvaluator& evaluator) final;

private:
	PreDirectiveType type;
	Position position;
	std::vector<PreToken> tokens;
};
