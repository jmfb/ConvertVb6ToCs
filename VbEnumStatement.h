#pragma once
#include "SentenceParser.h"

//<enum-statement> =
//	"enum" id <statement-separator> <enum-member-list-opt> "end" "enum"
//	| "public" "enum" id <statement-separator> <enum-member-list-opt> "end" "enum"
//	| "private" "enum" id <statement-separator> <enum-member-list-opt> "end" "enum";
//<enum-member-list> =
//	<enum-member> <enum-member-list'>;
//<enum-member-list'> =
//	<enum-member> <enum-member-list'>
//	| @;
class VbEnumStatement
{
public:
	VbEnumStatement(const Sentence& sentence)
	{
		std::tie(access, std::ignore, name, std::ignore, members, std::ignore, std::ignore) =
			SentenceParser::Parse(
				sentence,
				OptionalToken("public", "private"),
				RequiredToken("enum"),
				RequiredToken(),
				RequiredSentence("statement-separator"),
				OptionalSentenceList("enum-member"),
				RequiredToken("end"),
				RequiredToken("enum"));
	}

	optional<Token> access;
	Token name;
	optional<std::vector<Sentence>> members;
};
