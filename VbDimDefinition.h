#pragma once
#include "SentenceParser.h"

//<dim-definition> =
//	<key-withevents-opt> id <array-suffix-opt> <as-new-specifier-opt>;
class VbDimDefinition
{
public:
	VbDimDefinition(const Sentence& sentence)
	{
		std::tie(isWithEvents, name, arraySuffix, asNewSpecifier) =
			SentenceParser::Parse(
				sentence,
				OptionalToken("withevents"),
				RequiredToken(),
				OptionalSentence("array-suffix"),
				OptionalSentence("as-new-specifier"));
	}

	bool isWithEvents;
	Token name;
	optional<Sentence> arraySuffix;
	optional<Sentence> asNewSpecifier;
};
