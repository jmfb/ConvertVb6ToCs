#pragma once
#include "SentenceParser.h"

//<expression> =
//	<or-expression>;
class VbExpression
{
public:
	VbExpression(const Sentence& sentence)
	{
		std::tie(orExpression) = SentenceParser::Parse(sentence, RequiredSentence("or-expression"));
	}

	Sentence orExpression;
};
