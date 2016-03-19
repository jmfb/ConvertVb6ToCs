#pragma once
#include "PreIfType.h"
#include "Position.h"
#include "PreToken.h"
#include "PreLine.h"
#include <vector>
#include <deque>

class PreIfBlock
{
public:
	PreIfBlock(PreIfType type, const Position& position, const std::vector<PreToken>& condition)
		: type(type), position(position), condition(condition)
	{
	}

	PreIfType GetType() const
	{
		return type;
	}
	const Position& GetPosition() const
	{
		return position;
	}
	const std::vector<PreToken>& GetCondition() const
	{
		return condition;
	}
	const std::deque<PreLinePtr>& GetIfGroup() const
	{
		return ifGroup;
	}
	std::deque<PreLinePtr>& GetIfGroup()
	{
		return ifGroup;
	}

private:
	PreIfType type;
	Position position;
	std::vector<PreToken> condition;
	std::deque<PreLinePtr> ifGroup;
};
