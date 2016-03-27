#pragma once
#include "SentenceParser.h"

//#Artificial production from parsing <expression-list>
//<expression-item> =
//	<pass-by-opt> id ':=' <expression>
//	| <pass-by-opt> <expression>
//	| @
class VbExpressionItem
{
public:
	VbExpressionItem(const Sentence& sentence)
	{
		std::tie(passBy, name, std::ignore, expression) = SentenceParser::Parse(
			sentence,
			OptionalSentence("pass-by"),
			OptionalToken(),
			OptionalToken(":="),
			OptionalSentence("expression"));
	}

	optional<Sentence> passBy;
	optional<Token> name;
	optional<Sentence> expression;
};
