#pragma once
#include "SentenceParser.h"

//<type-statement> =
//	<id.type> id <statement-separator> <type-definition-list> "end" <id.type>
//	| "public" <id.type> id <statement-separator> <type-definition-list> "end" <id.type>
//	| "private" <id.type> id <statement-separator> <type-definition-list> "end" <id.type>;
//<type-definition-list> =
//	<type-definition> <type-definition-list'>;
//<type-definition-list'> =
//	<type-definition> <type-definition-list'>
//	| @;
class VbTypeStatement
{
public:
	VbTypeStatement(const Sentence& sentence)
	{
		std::tie(access, std::ignore, name, std::ignore, typeDefinitions, std::ignore, std::ignore) =
			SentenceParser::Parse(
				sentence,
				OptionalToken("public", "private"),
				RequiredToken("type"),
				RequiredToken(),
				RequiredSentence("statement-separator"),
				RequiredSentenceList("type-definition"),
				RequiredToken("end"),
				RequiredToken("type"));
	}

	optional<Token> access;
	Token name;
	std::vector<Sentence> typeDefinitions;
};
