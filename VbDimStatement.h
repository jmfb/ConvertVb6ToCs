#pragma once
#include "SentenceParser.h"

//<dim-statement> =
//	"dim" <dim-definition-list>;
//<static-statement> =
//	"static" <dim-definition-list>;
//<public-statement> =
//	"public" <dim-definition-list>
//	| <id.global> <dim-definition-list>;
//<private-statement> =
//	"private" <dim-definition-list>;
//<dim-definition-list> =
//	<dim-definition> <dim-definition-list'>;
//<dim-definition-list'> =
//	',' <dim-definition> <dim-definition-list'>
//	| @;
class VbDimStatement
{
public:
	VbDimStatement(const Sentence& sentence)
	{
		std::tie(access, dimDefinitions) = SentenceParser::Parse(
			sentence,
			RequiredToken("dim", "static", "public", "global", "private"),
			RequiredSentenceList("dim-definition-list", "dim-definition", ","));
	}

	Token access;
	std::vector<Sentence> dimDefinitions;
};
