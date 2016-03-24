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
				OptionalSentence("relational-expression"),
				OptionalToken(">=", ">", "<=", "<"),
				RequiredSentence("additive-expression"));
	}

	optional<Sentence> relationalExpression;
	optional<Token> op;
	Sentence additiveExpression;
};
