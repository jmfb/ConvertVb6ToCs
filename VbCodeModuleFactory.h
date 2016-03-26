#pragma once
#include "VbCodeModule.h"
#include "VbCodeConstant.h"
#include "Sentence.h"
#include <string>
#include <vector>

class VbCodeModuleFactory
{
public:
	VbCodeModule Create(const Sentence& sentence);

private:
	void LoadHeader(const Sentence& sentence);
	void ProcessDeclaration(const Sentence& sentence);
	void ProcessStatement(const Sentence& sentence);
	void ProcessHeaderStatement(const Sentence& sentence);
	void ProcessBodyStatement(const Sentence& sentence);
	void ProcessConstStatement(const Sentence& sentence);

private:
	bool isBeforeFirstFunction = true;

private:
	std::string name;
	bool isOptionExplicit = false;
	std::vector<VbCodeConstant> constants;
};
