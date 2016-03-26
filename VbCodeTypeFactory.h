#pragma once
#include "VbCodeType.h"
#include "Sentence.h"
#include "optional.h"

class VbCodeTypeFactory
{
public:
	static VbCodeType Create(const optional<Sentence>& sentence);
	static optional<VbCodeType> CreateOptional(const optional<Sentence>& sentence);
	static VbCodeType Create(const Sentence& sentence);
};
