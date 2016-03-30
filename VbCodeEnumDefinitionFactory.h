#pragma once
#include "VbCodeEnumDefinition.h"
#include "Sentence.h"

class VbCodeEnumDefinitionFactory
{
public:
	static VbCodeEnumDefinition Create(const Sentence& sentence);
};
