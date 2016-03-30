#pragma once
#include "SentenceParser.h"

//<enum-member> =
//	<statement-separator>
//	| id <statement-separator>
//	| id '=' <expression> <statement-separator>;
class VbEnumMember
{
public:
	VbEnumMember(const Sentence& sentence)
	{
		std::tie(name, std::ignore, expression, std::ignore) =
			SentenceParser::Parse(
				sentence,
				OptionalToken(),
				OptionalToken("="),
				OptionalSentence("expression"),
				RequiredSentence("statement-separator"));
	}

	optional<Token> name;
	optional<Sentence> expression;
};
