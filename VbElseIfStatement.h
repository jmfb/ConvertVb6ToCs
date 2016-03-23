#pragma once
#include "SentenceParser.h"

//<elseif-statement> =
//	"elseif" <expression> "then";
class VbElseIfStatement
{
public:
	VbElseIfStatement(const Sentence& sentence)
	{
		std::tie(std::ignore, expression, std::ignore) =
			SentenceParser::Parse(
				sentence,
				RequiredToken("elseif"),
				RequiredSentence("expression"),
				RequiredToken("then"));
	}

	Sentence expression;
};
