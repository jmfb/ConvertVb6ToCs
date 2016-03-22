#pragma once
#include "SentenceParser.h"

//<literal> =
//	val
//	| "true"
//	| "false"
//	| "empty"
//	| "nothing"
//	| "null";
class VbLiteral
{
public:
	VbLiteral(const Sentence& sentence)
	{
		std::tie(value) = SentenceParser::Parse(sentence, RequiredToken());
	}

	Token value;
};
