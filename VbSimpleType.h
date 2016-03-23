#pragma once
#include "SentenceParser.h"

//<simple-type> =
//	"byte"
//	| "boolean"
//	| "integer"
//	| "long"
//	| "currency"
//	| "single"
//	| "double"
//	| <id.date>
//	| "string"
//	| "string" '*' val
//	| "string" '*' <qualified-id>
//	| <id.object>
//	| "variant";
class VbSimpleType
{
public:
	VbSimpleType(const Sentence& sentence)
	{
		std::tie(type, std::ignore, constantSize, variableSize) =
			SentenceParser::Parse(
				sentence,
				RequiredToken(
					"byte",
					"boolean",
					"integer",
					"long",
					"currency",
					"single",
					"double",
					"date",
					"string",
					"object",
					"variant"),
				OptionalToken("*"),
				OptionalToken(),
				OptionalSentence("qualified-id"));
	}

	Token type;
	optional<Token> constantSize;
	optional<Sentence> variableSize;
};
