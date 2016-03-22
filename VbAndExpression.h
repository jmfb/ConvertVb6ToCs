#pragma once
#include "SentenceParser.h"

//<and-expression> =
//	<equality-expression>
//	| <and-expression> "and" <equality-expression>;
class VbAndExpression
{
public:
	VbAndExpression(const Sentence& sentence)
	{
		std::tie(andExpression, std::ignore, equalityExpression) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("and-expression"),
				OptionalToken("and"),
				RequiredSentence("equality-expression"));
	}

	optional<Sentence> andExpression;
	Sentence equalityExpression;
};
