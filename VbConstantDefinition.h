#pragma once
#include "SentenceParser.h"

//<constant-definition> =
//	id <as-specifier-opt> '=' <expression>;
class VbConstantDefinition
{
public:
	VbConstantDefinition(const Sentence& sentence)
	{
		std::tie(name, asSpecifier, std::ignore, expression) =
			SentenceParser::Parse(
				sentence,
				RequiredToken(),
				OptionalSentence("as-specifier"),
				RequiredToken("="),
				RequiredSentence("expression"));
	}

	Token name;
	optional<Sentence> asSpecifier;
	Sentence expression;
};
