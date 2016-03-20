#pragma once
#include "Token.h"
#include "Sentence.h"
#include <string>
#include <cstdlib>
#include <stdexcept>

class RequiredTokenAccessor
{
public:
	RequiredTokenAccessor(const std::string& value)
		: value(value)
	{
	}

	Token operator()(const Sentence& sentence, std::size_t index) const
	{
		if (index >= sentence.GetNodes().size())
			throw std::runtime_error("Index outside of sentence.");
		auto& token = sentence.GetNodes()[index]->AsToken();
		if (!value.empty() && token != value)
			throw std::runtime_error("Expected " + value);
		return token;
	}

private:
	std::string value;
};

inline RequiredTokenAccessor RequiredToken(const std::string& value = "")
{
	return{ value };
}
