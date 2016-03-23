#pragma once
#include "SentenceParser.h"

//<redim-definition> =
//	<qualified-id> '(' <subscript-list> ')' <as-specifier-opt>;
//<subscript-list> =
//	<subscript> <subscript-list'>;
//<subscript-list'> =
//	',' <subscript> <subscript-list'>
//	| @;
class VbRedimDefinition
{
public:
	VbRedimDefinition(const Sentence& sentence)
	{
		std::tie(qualifiedId, std::ignore, subscripts, std::ignore, asSpecifier) =
			SentenceParser::Parse(
				sentence,
				RequiredSentence("qualified-id"),
				RequiredToken("("),
				RequiredSentenceList("subscript-list", "subscript", ","),
				RequiredToken(")"),
				OptionalSentence("as-specifier"));
	}

	Sentence qualifiedId;
	std::vector<Sentence> subscripts;
	optional<Sentence> asSpecifier;
};
