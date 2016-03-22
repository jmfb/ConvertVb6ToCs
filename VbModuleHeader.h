#pragma once
#include "SentenceParser.h"

//<module-header> =
//	<attribute-list>;
//<attribute-list> =
//	<attribute> <attribute-list'>;
//<attribute-list'> =
//	<attribute> <attribute-list'>
//	| @;
class VbModuleHeader
{
public:
	VbModuleHeader(const Sentence& sentence)
	{
		std::tie(attributes) = SentenceParser::Parse(sentence, RequiredSentenceList("attribute"));
	}

	std::vector<Sentence> attributes;
};
