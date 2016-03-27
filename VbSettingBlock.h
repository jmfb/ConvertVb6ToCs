#pragma once
#include "SentenceParser.h"

//<setting-block> =
//	<id.begin> '\n' <setting-list-opt> "end" '\n';
//<setting-list> =
//	<setting> <setting-list-opt>;
class VbSettingBlock
{
public:
	VbSettingBlock(const Sentence& sentence)
	{
		std::tie(std::ignore, std::ignore, settings, std::ignore, std::ignore) =
			SentenceParser::Parse(
				sentence,
				RequiredToken("begin"),
				RequiredToken("\\n"),
				OptionalSentenceList("setting"),
				RequiredToken("end"),
				RequiredToken("\\n"));
	}

	optional<std::vector<Sentence>> settings;
};
