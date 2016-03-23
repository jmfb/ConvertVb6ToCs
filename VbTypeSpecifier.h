#pragma once
#include "SentenceParser.h"

//<type-specifier> =
//	<qualified-id>
//	| <simple-type>;
class VbTypeSpecifier
{
public:
	VbTypeSpecifier(const Sentence& sentence)
	{
		std::tie(qualifiedId, simpleType) = SentenceParser::Parse(
			sentence,
			OptionalSentence("qualified-id"),
			OptionalSentence("simple-type"));
	}

	optional<Sentence> qualifiedId;
	optional<Sentence> simpleType;
};
