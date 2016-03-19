#pragma once
#include "PreToken.h"

class VbPreTokenFactory
{
public:
	static PreToken Create(PreTokenType type, const Position& position, const std::string& value)
	{
		return{ type, position, value };
	}
};
