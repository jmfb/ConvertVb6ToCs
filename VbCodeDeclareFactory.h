#pragma once
#include "VbCodeDeclare.h"
#include "Sentence.h"

class VbCodeDeclareFactory
{
public:
	static VbCodeDeclare Create(const Sentence& sentence);
};
