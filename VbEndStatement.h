#pragma once
#include "SentenceParser.h"
#include "VbEndType.h"

//<end-statement> =
//	"end" <end-keyword-opt>;
//<end-keyword> =
//	#<id.type> and "enum" handled specially by begin statement
//	"sub"
//	| "function"
//	| <id.property>
//	| "if"
//	| <id.select>
//	| "with";
class VbEndStatement
{
public:
	VbEndStatement(const Sentence& sentence)
	{
		optional<Sentence> keyword;
		std::tie(std::ignore, keyword) = SentenceParser::Parse(
			sentence,
			RequiredToken("end"),
			OptionalSentence("end-keyword"));
		type = ParseType(keyword);
	}

	VbEndType ParseType(const optional<Sentence>& keyword)
	{
		if (!keyword)
			return VbEndType::Program;
		return SentenceParser::ToEnum<VbEndType>(
			std::get<0>(SentenceParser::Parse(*keyword, RequiredToken())),
			{
				{ "sub", VbEndType::Sub },
				{ "function", VbEndType::Function },
				{ "property", VbEndType::Property },
				{ "if", VbEndType::If },
				{ "select", VbEndType::Select },
				{ "with", VbEndType::With }
			});
	}

	VbEndType type;
};
