#pragma once
#include "SentenceParser.h"

//<redim-statement> =
//	"redim" <key-preserve-opt> <redim-definition-list>;
//<redim-definition-list> =
//	<redim-definition> <redim-definition-list'>;
//<redim-definition-list'> =
//	',' <redim-definition> <redim-definition-list'>
//	| @;
class VbRedimStatement
{
public:
	VbRedimStatement(const Sentence& sentence)
	{
		std::tie(std::ignore, preserve, redimDefinitions) =
			SentenceParser::Parse(
				sentence,
				RequiredToken("redim"),
				OptionalToken("preserve"),
				RequiredSentenceList("redim-definition-list", "redim-definition", ","));
	}

	bool preserve;
	std::vector<Sentence> redimDefinitions;
};
