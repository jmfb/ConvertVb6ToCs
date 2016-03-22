#pragma once
#include "SentenceParser.h"

//<unary-expression> =
//	<postfix-expression>
//	| "new" <qualified-id>
//	| "addressof" <qualified-id>
//	| "typeof" <l-value> "is" <qualified-id>
//	| "string" <expression-clause>
//	| "len" <expression-clause>
//	| "not" <unary-expression>
//	| '-' <unary-expression>
//	| '#' <unary-expression>;
class VbUnaryExpression
{
public:
	VbUnaryExpression(const Sentence& sentence)
	{
		std::tie(postfixExpression, op, lValue, std::ignore, qualifiedId, expressionClause, unaryExpression) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("postfix-expression"),
				OptionalToken("new", "addressof", "typeof", "string", "len", "not", "-", "#"),
				OptionalSentence("l-value"),
				OptionalToken("is"),
				OptionalSentence("qualified-id"),
				OptionalSentence("expression-clause"),
				OptionalSentence("unary-expression"));
	}

	optional<Sentence> postfixExpression;
	optional<Token> op;
	optional<Sentence> lValue;
	optional<Sentence> qualifiedId;
	optional<Sentence> expressionClause;
	optional<Sentence> unaryExpression;
};
