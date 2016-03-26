#pragma once
#include "VbCodeTypeName.h"
#include "Sentence.h"

class VbCodeTypeNameFactory
{
public:
	static VbCodeTypeName Create(const Sentence& sentence);
};
