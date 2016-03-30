#pragma once
#include "SentenceParser.h"

//<case-clause> =
//	"else"
//	| <case-expression-list>;
//<case-expression-list> =
//	<case-expression> <case-expression-list'>;
//<case-expression-list'> =
//	',' <case-expression> <case-expression-list'>
//	| @;
class VbCaseClause
{
public:
	VbCaseClause(const Sentence& sentence)
	{
		std::tie(std::ignore, caseExpressions) = SentenceParser::Parse(
			sentence,
			OptionalToken("else"),
			OptionalSentenceList("case-expression-list", "case-expression", ","));
	}

	optional<std::vector<Sentence>> caseExpressions;
};
