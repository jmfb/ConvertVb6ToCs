#pragma once
#include "SentenceParser.h"

//<xor-expression> =
//	<and-expression>
//	| <xor-expression> "xor" <and-expression>;
class VbXorExpression
{
public:
	VbXorExpression(const Sentence& sentence)
	{
		std::tie(xorExpression, std::ignore, andExpression) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("xor-expression"),
				OptionalToken("xor"),
				RequiredSentence("and-expression"));
	}

	optional<Sentence> xorExpression;
	Sentence andExpression;
};
