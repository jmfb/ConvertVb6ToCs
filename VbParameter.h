#pragma once
#include "SentenceParser.h"

//<parameter> =
//	<key-optional-opt> <pass-by-opt> <key-paramarray-opt> id <array-specifier-opt> <as-specifier-opt> <default-value-opt>;
//<array-specifier> =
//	'(' ')';
class VbParameter
{
public:
	VbParameter(const Sentence& sentence)
	{
		std::tie(isOptional, passBy, isParamArray, name, isArray, asSpecifier, defaultValue) =
			SentenceParser::Parse(
				sentence,
				OptionalToken("optional"),
				OptionalSentence("pass-by"),
				OptionalToken("paramarray"),
				RequiredToken(),
				OptionalSentence("array-specifier"),
				OptionalSentence("as-specifier"),
				OptionalSentence("default-value"));
	}

	bool isOptional;
	optional<Sentence> passBy;
	bool isParamArray;
	Token name;
	bool isArray;
	optional<Sentence> asSpecifier;
	optional<Sentence> defaultValue;
};
