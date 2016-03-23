#pragma once
#include "SentenceParser.h"

//<l-value-list> =
//	<l-value> <l-value-list'>;
//<l-value-list'> =
//	',' <l-value> <l-value-list'>
//	| @;
class VbLValueList
{
public:
	VbLValueList(const Sentence& sentence)
	{
		for (auto& node : sentence.GetNodes())
		{
			if (node->IsSentence())
			{
				auto& item = node->AsSentence();
				if (item.GetName() != "l-value")
					throw std::runtime_error("Expected l-value.");
				lValues.push_back(item);
			}
			else
			{
				if (node->AsToken().GetValue() != ",")
					throw std::runtime_error("Expected comma.");
			}
		}
	}

	std::vector<Sentence> lValues;
};
