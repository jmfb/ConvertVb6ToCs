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
	OptionalSentenceListAccessor(
		const std::string& listName,
		const std::string& itemName,
		const std::string& separator)
		: listName(listName), itemName(itemName), separator(separator)
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
			if (node->IsSentence())
			{
				auto& item = node->AsSentence();
				if (item.GetName() != itemName)
					throw std::runtime_error("Expected: " + itemName);
				list.push_back(item);
			}
			else
			{
				auto& token = node->AsToken();
				if (separator.empty() || token != separator)
					throw std::runtime_error("Unexpected sentence list separator.");
			}
		}
		return list;
	}

private:
	std::string listName;
	std::string itemName;
	std::string separator;
};

inline OptionalSentenceListAccessor OptionalSentenceList(const std::string& name)
{
	return{ name + "-list", name, "" };
}

inline OptionalSentenceListAccessor OptionalSentenceList(
	const std::string& listName,
	const std::string& itemName,
	const std::string& separator = "")
{
	return{ listName, itemName, separator };
}
