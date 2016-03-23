#pragma once
#include "SentenceParser.h"

//<type-definition> =
//	<statement-separator>
//	| id <array-suffix-opt> <as-specifier> <statement-separator>;
class VbTypeDefinition
{
public:
	VbTypeDefinition(const Sentence& sentence)
	{
		std::tie(name, arraySuffix, asSpecifier, std::ignore) =
			SentenceParser::Parse(
				sentence,
				OptionalToken(),
				OptionalSentence("array-suffix"),
				OptionalSentence("as-specifier"),
				RequiredSentence("statement-separator"));
	}

	optional<Token> name;
	optional<Sentence> arraySuffix;
	optional<Sentence> asSpecifier;
};
