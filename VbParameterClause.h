#pragma once
#include "SentenceParser.h"

//<parameter-clause> =
//	'(' <parameter-list-opt> ')'
//	| ' (' <parameter-list-opt> ')';
//<parameter-list> =
//	<parameter> <parameter-list'>;
//<parameter-list'> =
//	',' <parameter> <parameter-list'>
//	| @;
class VbParameterClause
{
public:
	VbParameterClause(const Sentence& sentence)
	{
		std::tie(std::ignore, parameters, std::ignore) = SentenceParser::Parse(
			sentence,
			RequiredToken("(", " ("),
			OptionalSentenceList("parameter-list", "parameter", ","),
			RequiredToken(")"));
	}

	optional<std::vector<Sentence>> parameters;
};
