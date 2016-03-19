#pragma once
#include "SentenceParser.h"

//<let-statement> =
//	"let" <l-value> '=' <expression>
//	| <l-value> '=' <expression>;
class VbLetStatement
{
public:
	VbLetStatement(const Sentence& sentence)
		: VbLetStatement(SentenceParser::Parse(sentence,
			OptionalToken("let"),
			RequiredSentence("l-value"),
			RequiredToken("="),
			RequiredSentence("expression")))
	{
	}

private:
	template <typename Tuple>
	VbLetStatement(const Tuple& result)
		: lValue(std::get<1>(result)),
		expression(std::get<3>(result))
	{
	}

public:
	Sentence lValue;
	Sentence expression;
};
