#pragma once
#include "SentenceParser.h"

//<subscript> =
//	<expression>
//	| <expression> "to" <expression>;
class VbSubscript
{
public:
	VbSubscript(const Sentence& sentence)
	{
		std::tie(expression1, std::ignore, expression2) =
			SentenceParser::Parse(
				sentence,
				RequiredSentence("expression"),
				OptionalToken("to"),
				OptionalSentence("expression"));
	}

	Sentence expression1;
	optional<Sentence> expression2;
};
