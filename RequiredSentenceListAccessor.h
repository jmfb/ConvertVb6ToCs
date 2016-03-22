#pragma once
#include "Sentence.h"
#include <string>
#include <vector>
#include <stdexcept>

class RequiredSentenceListAccessor
{
public:
	RequiredSentenceListAccessor(const std::string& name)
		: name(name)
	{
	}

	std::vector<Sentence> operator()(const Sentence& sentence, std::size_t index) const
	{
		if (index >= sentence.GetNodes().size())
			throw std::runtime_error("Index out of sentence.");
		auto& child = sentence.GetNodes()[index]->AsSentence();
		if (child.GetName() != (name + "-list"))
			throw std::runtime_error("Expected: " + name + "-list");
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

inline RequiredSentenceListAccessor RequiredSentenceList(const std::string& name)
{
	return{ name };
}
