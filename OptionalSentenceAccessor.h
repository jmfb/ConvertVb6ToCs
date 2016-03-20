#pragma once
#include "Sentence.h"
#include "optional.h"
#include <string>
#include <cstdlib>

class OptionalSentenceAccessor
{
public:
	OptionalSentenceAccessor(const std::string& name)
		: name(name)
	{
	}

	optional<Sentence> operator()(const Sentence& sentence, std::size_t index) const
	{
		if (index >= sentence.GetNodes().size() || !sentence.GetNodes()[index]->IsSentence())
			return{};
		auto& child = sentence.GetNodes()[index]->AsSentence();
		if (name.empty() || child.GetName() == name)
			return child;
		return{};
	}

private:
	std::string name;
};

inline OptionalSentenceAccessor OptionalSentence(const std::string& name = "")
{
	return{ name };
}
