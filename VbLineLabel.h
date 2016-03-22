#pragma once
#include "SentenceParser.h"

//<line-label> =
//	id ':'      #Ambiguity resolution via ConflictResolver
//	| val ':';
class VbLineLabel
{
public:
	VbLineLabel(const Sentence& sentence)
	{
		std::tie(label, std::ignore) = SentenceParser::Parse(
			sentence,
			RequiredToken(),
			RequiredToken(":"));
	}

	Token label;
};
