#pragma once
#include "SentenceParser.h"

//<equality-expression> =
//	<relational-expression>
//	| <equality-expression> '=' <relational-expression>
//	| <equality-expression> '<>' <relational-expression>
//	| <equality-expression> "imp" <relational-expression>
//	| <equality-expression> "eqv" <relational-expression>
//	| <equality-expression> "is" <relational-expression>
//	| <equality-expression> "like" <relational-expression>;
class VbEqualityExpression
{
public:
	VbEqualityExpression(const Sentence& sentence)
	{
		std::tie(equalityExpression, op, relationalExpression) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("equality-expression"),
				OptionalToken("=", "<>", "imp", "eqv", "is", "like"),
				RequiredSentence("relational-expression"));
	}

	optional<Sentence> equalityExpression;
	optional<Token> op;
	Sentence relationalExpression;
};
