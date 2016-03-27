#pragma once
#include "SentenceParser.h"

//<setting-value> =
//	<expression>
//	| val ':' val
//	| val ':' id
//	| '$' val ':' val
//	| '$' val ':' id;
class VbSettingValue
{
public:
	VbSettingValue(const Sentence& sentence)
	{
		std::tie(expression, std::ignore, name, std::ignore, value) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("expression"),
				OptionalToken("$"),
				OptionalToken(),
				OptionalToken(":"),
				OptionalToken());
	}

	optional<Sentence> expression;
	optional<Token> name;
	optional<Token> value;
};
