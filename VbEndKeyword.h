#pragma once
#include "SentenceParser.h"

//<end-keyword> =
//	#<id.type> and "enum" handled specially by begin statement
//	"sub"
//	| "function"
//	| <id.property>
//	| "if"
//	| <id.select>
//	| "with";
class VbEndKeyword
{
public:
	VbEndKeyword(const Sentence& sentence)
	{
		std::tie(keyword) = SentenceParser::Parse(
			sentence,
			RequiredToken("sub", "function", "property", "if", "select", "with"));
	}

	Token keyword;
};
