#pragma once
#include "SentenceParser.h"

//<or-expression> =
//	<xor-expression>
//	| <or-expression> "or" <xor-expression>;
class VbOrExpression
{
public:
	VbOrExpression(const Sentence& sentence)
	{
		std::tie(orExpression, std::ignore, xorExpression) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("or-expression"),
				OptionalToken("or"),
				RequiredSentence("xor-expression"));
	}

	optional<Sentence> orExpression;
	Sentence xorExpression;
};
