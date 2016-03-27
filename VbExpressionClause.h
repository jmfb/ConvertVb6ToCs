#pragma once
#include "SentenceParser.h"

//<expression-clause> =
//	'(' <expression-list-opt> ')';
class VbExpressionClause
{
public:
	VbExpressionClause(const Sentence& sentence)
	{
		std::tie(std::ignore, expressionList, std::ignore) = SentenceParser::Parse(
			sentence,
			RequiredToken("("),
			OptionalSentence("expression-list"),
			RequiredToken(")"));
	}

	optional<Sentence> expressionList;
};
