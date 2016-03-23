#pragma once
#include "SentenceParser.h"

//<call-suffix> =
//	<call-terminal> <call-suffix'>;
//<call-suffix'> =
//	<dot> <call-terminal> <call-suffix'>
//	| @;
class VbCallSuffix
{
public:
	VbCallSuffix(const Sentence& sentence)
	{
		if ((sentence.GetNodes().size() % 2) != 1)
			throw std::runtime_error("Call suffix must be odd number in size.");
		callTerminal = sentence.GetNodes()[0]->AsSentence();
		if (callTerminal.GetName() != "call-terminal")
			throw std::runtime_error("Expected call-terminal.");
		for (auto index = 1u; index < sentence.GetNodes().size(); index += 2)
		{
			auto& dot = sentence.GetNodes()[index]->AsSentence();
			if (dot.GetName() != "dot")
				throw std::runtime_error("Expected dot");
			auto& nextCallTerminal = sentence.GetNodes()[index + 1]->AsSentence();
			if (nextCallTerminal.GetName() != "call-terminal")
				throw std::runtime_error("Expected call-terminal");
			suffix.emplace_back(dot, nextCallTerminal);
		}
	}

	Sentence callTerminal;
	std::vector<std::pair<Sentence, Sentence>> suffix;
};
