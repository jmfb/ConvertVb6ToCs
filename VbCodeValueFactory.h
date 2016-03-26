#pragma once
#include "VbCodeValue.h"
#include "Token.h"

class VbCodeValueFactory
{
public:
	static VbCodeValue Create(const Token& token);
};
