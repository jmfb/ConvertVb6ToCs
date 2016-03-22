#pragma once
#include "SentenceParser.h"

//<postfix-expression> =
//	<primary-expression>
//	| <postfix-expression> <dot> id
//	| <postfix-expression> <expression-clause>;
class VbPostfixExpression
{
public:
	VbPostfixExpression(const Sentence& sentence)
	{
		std::tie(postfixExpression, primaryExpression, dot, id, expressionClause) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("primary-expression"),
				OptionalSentence("postfix-expression"),
				OptionalSentence("dot"),
				OptionalToken(),
				OptionalSentence("expression-clause"));
	}

	optional<Sentence> postfixExpression;
	optional<Sentence> primaryExpression;
	optional<Sentence> dot;
	optional<Token> id;
	optional<Sentence> expressionClause;
};
