#pragma once
#include "SentenceParser.h"

//<line-label> =
//	id ':'      #Ambiguity resolution via ConflictResolver
//	| val ':';
class VbLineLabel
{
public:
	VbLineLabel(const Sentence& sentence)
		: VbLineLabel(SentenceParser::Parse(sentence,
			RequiredToken(),
			RequiredToken(":")))
	{
	}

private:
	template <typename Tuple>
	VbLineLabel(const Tuple& result)
		: label(std::get<0>(result))
	{
	}

public:
	Token label;
};
