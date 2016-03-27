#pragma once
#include "VbCodeClass.h"
#include "Sentence.h"
#include <string>

class VbCodeClassFactory
{
public:
	VbCodeClass Create(const std::string& library, const Sentence& sentence);

private:
	void LoadHeader(const Sentence& sentence);

private:
	std::string name;
};
