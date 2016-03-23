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
	OptionalSentenceListAccessor(const std::string& listName, const std::string& itemName)
		: listName(listName), itemName(itemName)
	{
	}

	optional<std::vector<Sentence>> operator()(const Sentence& sentence, std::size_t index) const
	{
		if (index >= sentence.GetNodes().size() || !sentence.GetNodes()[index]->IsSentence())
			return{};
		auto& child = sentence.GetNodes()[index]->AsSentence();
		if (child.GetName() != listName)
			return{};
		std::vector<Sentence> list;
		for (auto& node : child.GetNodes())
		{
			auto& item = node->AsSentence();
			if (item.GetName() != itemName)
				throw std::runtime_error("Expected: " + itemName);
			list.push_back(item);
		}
		return list;
	}

private:
	std::string listName;
	std::string itemName;
};

inline OptionalSentenceListAccessor OptionalSentenceList(const std::string& name)
{
	return{ name + "-list", name };
}

inline OptionalSentenceListAccessor OptionalSentenceList(const std::string& listName, const std::string& itemName)
{
	return{ listName, itemName };
}
