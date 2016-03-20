#pragma once
#include "optional.h"
#include "Sentence.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>

class OptionalSentenceListAccessor
{
public:
	OptionalSentenceListAccessor(const std::string& name)
		: name(name)
	{
	}

	optional<std::vector<Sentence>> operator()(const Sentence& sentence, std::size_t index) const
	{
		if (index >= sentence.GetNodes().size() || !sentence.GetNodes()[index]->IsSentence())
			return{};
		auto& child = sentence.GetNodes()[index]->AsSentence();
		if (child.GetName() != (name + "-list"))
			return{};
		std::vector<Sentence> list;
		for (auto& node : child.GetNodes())
		{
			auto& item = node->AsSentence();
			if (item.GetName() != name)
				throw std::runtime_error("Expected: " + name);
			list.push_back(item);
		}
		return list;
	}

private:
	std::string name;
};

inline OptionalSentenceListAccessor OptionalSentenceList(const std::string& name)
{
	return{ name };
}
