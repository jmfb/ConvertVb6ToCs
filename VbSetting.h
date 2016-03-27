#pragma once
#include "SentenceParser.h"

//<setting> =
//	<l-value> '=' <setting-value> '\n';
class VbSetting
{
public:
	VbSetting(const Sentence& sentence)
	{
		std::tie(lValue, std::ignore, settingValue, std::ignore) =
			SentenceParser::Parse(
				sentence,
				RequiredSentence("l-value"),
				RequiredToken("="),
				RequiredSentence("setting-value"),
				RequiredToken("\\n"));
	}

	Sentence lValue;
	Sentence settingValue;
};
