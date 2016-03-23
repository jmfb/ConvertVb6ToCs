#pragma once
#include "SentenceParser.h"

//<l-value-terminal> =
//	<dot> id
//	| <expression-clause>;
class VbLValueTerminal
{
public:
	VbLValueTerminal(const Sentence& sentence)
	{
		std::tie(dot, id, expressionClause) = SentenceParser::Parse(
			sentence,
			OptionalSentence("dot"),
			OptionalToken(),
			OptionalSentence("expression-clause"));
	}

	optional<Sentence> dot;
	optional<Token> id;
	optional<Sentence> expressionClause;
};
