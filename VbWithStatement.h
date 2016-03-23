#pragma once
#include "SentenceParser.h"

//<with-statement> =
//	"with" <l-value>;
class VbWithStatement
{
public:
	VbWithStatement(const Sentence& sentence)
	{
		std::tie(std::ignore, lValue) = SentenceParser::Parse(
			sentence,
			RequiredToken("with"),
			RequiredSentence("l-value"));
	}

	Sentence lValue;
};
