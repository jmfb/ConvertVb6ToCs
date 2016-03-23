#pragma once
#include "Token.h"
#include "Sentence.h"
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <initializer_list>
#include <set>

class RequiredTokenAccessor
{
public:
	RequiredTokenAccessor(const std::initializer_list<std::string>& values)
		: values(values)
	{
	}

	Token operator()(const Sentence& sentence, std::size_t index) const
	{
		if (index >= sentence.GetNodes().size())
			throw std::runtime_error("Index outside of sentence.");
		auto& token = sentence.GetNodes()[index]->AsToken();
		if (values.empty())
			return token;
		for (auto& value : values)
			if (token == value)
				return token;
		throw std::runtime_error("Unexpected token.");
	}

private:
	std::set<std::string> values;
};

template <typename... String>
inline RequiredTokenAccessor RequiredToken(String... values)
{
	return{ values... };
}
