#pragma once
#include "VbCodeParameter.h"
#include "Sentence.h"
#include "optional.h"
#include <vector>

class VbCodeParameterFactory
{
public:
	static std::vector<VbCodeParameter> Create(const optional<Sentence>& sentence);
	static std::vector<VbCodeParameter> Create(const Sentence& sentence);
};
