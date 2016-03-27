#pragma once
#include "SentenceParser.h"

//<class-header> =
//	<id.version> val <id.class> '\n' <setting-block> <attribute-list>;
//<attribute-list> =
//	<attribute> <attribute-list'>;
//<attribute-list'> =
//	<attribute> <attribute-list'>
//	| @;
class VbClassHeader
{
public:
	VbClassHeader(const Sentence& sentence)
	{
		std::tie(std::ignore, std::ignore, std::ignore, std::ignore, settingBlock, attributes) =
			SentenceParser::Parse(
				sentence,
				RequiredToken("version"),
				RequiredToken(),
				RequiredToken("class"),
				RequiredToken("\\n"),
				RequiredSentence("setting-block"),
				RequiredSentenceList("attribute"));
	}

	Sentence settingBlock;
	std::vector<Sentence> attributes;
};
