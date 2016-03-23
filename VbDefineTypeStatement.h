#pragma once
#include "SentenceParser.h"

//<define-type-statement> =
//	<define-type-keyword> <letter-range-list>;
class VbDefineTypeStatement
{
public:
	VbDefineTypeStatement(const Sentence& sentence)
	{
		std::tie(keyword, letterRanges) = SentenceParser::Parse(
			sentence,
			RequiredSentence("define-type-keyword"),
			RequiredSentence("letter-range-list"));
	}

	Sentence keyword;
	Sentence letterRanges;
};
