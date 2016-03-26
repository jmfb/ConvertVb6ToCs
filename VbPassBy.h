#pragma once
#include "SentenceParser.h"

//<pass-by> =
//	"byref"
//	| "byval";
class VbPassBy
{
public:
	VbPassBy(const Sentence& sentence)
	{
		std::tie(passBy) = SentenceParser::Parse(sentence, RequiredToken("byval", "byref"));
	}

	Token passBy;
};
