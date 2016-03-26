#pragma once
#include "VbCodeTypeDefinition.h"
#include "Sentence.h"

class VbCodeTypeDefinitionFactory
{
public:
	static VbCodeTypeDefinition Create(const Sentence& sentence);
};
