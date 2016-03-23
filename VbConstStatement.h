#pragma once
#include "SentenceParser.h"

//<const-statement> =
//	"private" "const" <constant-definition-list>
//	| "public" "const" <constant-definition-list>
//	| <id.global> "const" <constant-definition-list>
//	| "const" <constant-definition-list>;
//<constant-definition-list> =
//	<constant-definition> <constant-definition-list'>;
//<constant-definition-list'> =
//	',' <constant-definition> <constant-definition-list'>
//	| @;
class VbConstStatement
{
public:
	VbConstStatement(const Sentence& sentence)
	{
		std::tie(access, std::ignore, constantDefinitions) =
			SentenceParser::Parse(
				sentence,
				OptionalToken("public", "private", "global"),
				RequiredToken("const"),
				RequiredSentenceList("constant-definition-list", "constant-definition", ","));
	}

	optional<Token> access;
	std::vector<Sentence> constantDefinitions;
};
