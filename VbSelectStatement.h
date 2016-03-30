#pragma once
#include "SentenceParser.h"

//<select-statement> =
//	<id.select> "case" <expression>;
class VbSelectStatement
{
public:
	VbSelectStatement(const Sentence& sentence)
	{
		std::tie(std::ignore, std::ignore, expression) = SentenceParser::Parse(
			sentence,
			RequiredToken("select"),
			RequiredToken("case"),
			RequiredSentence("expression"));
	}

	Sentence expression;
};
