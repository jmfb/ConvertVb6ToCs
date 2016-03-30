#pragma once
#include "SentenceParser.h"

//<set-statement> =
//	"set" <l-value> '=' <expression>;
class VbSetStatement
{
public:
	VbSetStatement(const Sentence& sentence)
	{
		std::tie(std::ignore, lValue, std::ignore, expression) =
			SentenceParser::Parse(
				sentence,
				RequiredToken("set"),
				RequiredSentence("l-value"),
				RequiredToken("="),
				RequiredSentence("expression"));
	}

	Sentence lValue;
	Sentence expression;
};
