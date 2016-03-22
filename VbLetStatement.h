#pragma once
#include "SentenceParser.h"

//<let-statement> =
//	"let" <l-value> '=' <expression>
//	| <l-value> '=' <expression>;
class VbLetStatement
{
public:
	VbLetStatement(const Sentence& sentence)
	{
		std::tie(std::ignore, lValue, std::ignore, expression) =
			SentenceParser::Parse(
				sentence,
				OptionalToken("let"),
				RequiredSentence("l-value"),
				RequiredToken("="),
				RequiredSentence("expression"));
	}

	Sentence lValue;
	Sentence expression;
};
