#pragma once
#include "SentenceParser.h"

//<case-expression> =
//	"is" <relational-op> <expression>
//	| <expression> "to" <expression>
//	| <expression>;
class VbCaseExpression
{
public:
	VbCaseExpression(const Sentence& sentence)
	{
		std::tie(std::ignore, relationalOp, expression1, std::ignore, expression2) =
			SentenceParser::Parse(
				sentence,
				OptionalToken("is"),
				OptionalSentence("relational-op"),
				RequiredSentence("expression"),
				OptionalToken("to"),
				OptionalSentence("expression"));
	}

	optional<Sentence> relationalOp;
	Sentence expression1;
	optional<Sentence> expression2;
};
