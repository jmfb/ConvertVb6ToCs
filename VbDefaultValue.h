#pragma once
#include "SentenceParser.h"

//<default-value> =
//	'=' <expression>;
class VbDefaultValue
{
public:
	VbDefaultValue(const Sentence& sentence)
	{
		std::tie(std::ignore, expression) = SentenceParser::Parse(
			sentence,
			RequiredToken("="),
			RequiredSentence("expression"));
	}

	Sentence expression;
};
