#pragma once
#include "VbCodeType.h"
#include "Sentence.h"

class VbCodeTypeFactory
{
public:
	static VbCodeType Create(const Sentence& sentence);
};
