#pragma once
#include "SentenceParser.h"

//<call-terminal> =
//	id
//	| id <expression-clause>;
class VbCallTerminal
{
public:
	VbCallTerminal(const Sentence& sentence)
	{
		std::tie(name, expressionClause) = SentenceParser::Parse(
			sentence,
			RequiredToken(),
			OptionalSentence("expression-clause"));
	}

	Token name;
	optional<Sentence> expressionClause;
};
