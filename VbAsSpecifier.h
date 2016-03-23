#pragma once
#include "SentenceParser.h"

//<as-specifier> =
//	"as" <type-specifier>;
//<as-new-specifier> =
//	"as" <key-new-opt> <type-specifier>;
//<as-array-specifier> =
//	"as" <type-specifier> <array-specifier-opt>;
class VbAsSpecifier
{
public:
	VbAsSpecifier(const Sentence& sentence)
	{
		std::tie(std::ignore, isNew, typeSpecifier, arraySpecifier) =
			SentenceParser::Parse(
				sentence,
				RequiredToken("as"),
				OptionalToken("new"),
				RequiredSentence("type-specifier"),
				OptionalSentence("array-specifier"));
	}

	bool isNew;
	Sentence typeSpecifier;
	optional<Sentence> arraySpecifier;
};
