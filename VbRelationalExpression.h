#pragma once
#include "SentenceParser.h"

//<relational-expression> =
//	<additive-expression>
//	| <relational-expression> '>=' <additive-expression>
//	| <relational-expression> '>' <additive-expression>
//	| <relational-expression> '<=' <additive-expression>
//	| <relational-expression> '<' <additive-expression>;
class VbRelationalExpression
{
public:
	VbRelationalExpression(const Sentence& sentence)
	{
		std::tie(relationalExpression, op, additiveExpression) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("relationalExpression"),
				OptionalToken(">=", ">", "<=", "<"),
				RequiredSentence("additiveExpression"));
	}

	optional<Sentence> relationalExpression;
	optional<Token> op;
	Sentence additiveExpression;
};
