#pragma once
#include "SentenceParser.h"

//<additive-expression> =
//	<multiplicative-expression>
//	| <additive-expression> '+' <multiplicative-expression>
//	| <additive-expression> '-' <multiplicative-expression>
//	| <additive-expression> '&' <multiplicative-expression>;
class VbAdditiveExpression
{
public:
	VbAdditiveExpression(const Sentence& sentence)
	{
		std::tie(additiveExpression, op, multiplicativeExpression) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("additive-expression"),
				OptionalToken("+", "-", "&"),
				RequiredSentence("multiplicative-expression"));
	}

	optional<Sentence> additiveExpression;
	optional<Token> op;
	Sentence multiplicativeExpression;
};
