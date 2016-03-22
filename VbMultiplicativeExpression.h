#pragma once
#include "SentenceParser.h"

//<multiplicative-expression> =
//	<unary-expression>
//	| <multiplicative-expression> '*' <unary-expression>
//	| <multiplicative-expression> '/' <unary-expression>
//	| <multiplicative-expression> '\' <unary-expression>
//	| <multiplicative-expression> "mod" <unary-expression>
//	| <multiplicative-expression> '^' <unary-expression>;
class VbMultiplicativeExpression
{
public:
	VbMultiplicativeExpression(const Sentence& sentence)
	{
		std::tie(multiplicativeExpression, op, unaryExpression) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("multiplicative-expression"),
				OptionalToken("*", "/", "\\", "mod", "^"),
				RequiredSentence("unary-expression"));
	}

	optional<Sentence> multiplicativeExpression;
	optional<Token> op;
	Sentence unaryExpression;
};
