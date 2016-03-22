#pragma once
#include "SentenceParser.h"

//<primary-expression> =
//	id
//	| <ws-dot> id
//	| "me"
//	| <literal>
//	| ' (' <expression> ')'
//	| ' (' <expression> ',' <expression> ')';   #Special case 'coordinate' expression for Line statements
class VbPrimaryExpression
{
public:
	VbPrimaryExpression(const Sentence& sentence)
	{
		std::tie(wsDot, id, literal, expression1, std::ignore, expression2, std::ignore) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("ws-dot"),
				OptionalToken(),
				OptionalSentence("literal"),
				OptionalSentence("expression"),
				OptionalToken(","),
				OptionalSentence("expression"),
				OptionalToken(")"));
	}

	optional<Sentence> wsDot;
	optional<Token> id;
	optional<Sentence> literal;
	optional<Sentence> expression1;
	optional<Sentence> expression2;
};
