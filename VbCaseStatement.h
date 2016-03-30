#pragma once
#include "SentenceParser.h"

//<case-statement> =
//	"case" <case-clause>;
class VbCaseStatement
{
public:
	VbCaseStatement(const Sentence& sentence)
	{
		std::tie(std::ignore, caseClause) = SentenceParser::Parse(
			sentence,
			RequiredToken("case"),
			RequiredSentence("case-clause"));
	}

	Sentence caseClause;
};
