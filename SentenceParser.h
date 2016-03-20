#pragma once
#include "Sentence.h"
#include "optional.h"
#include "OptionalTokenAccessor.h"
#include "RequiredTokenAccessor.h"
#include "OptionalSentenceAccessor.h"
#include "RequiredSentenceAccessor.h"
#include "OptionalSentenceListAccessor.h"
#include <string>
#include <map>
#include <stdexcept>
#include <cstdlib>
#include <tuple>

class SentenceParser
{
public:
	template <typename Enum>
	static Enum ToEnum(const Token& token, const std::map<std::string, Enum>& map)
	{
		for (auto& entry : map)
			if (token == entry.first)
				return entry.second;
		throw std::runtime_error("Invalid enum value.");
	}

	template <typename Enum>
	static Enum ToEnum(const optional<Token>& token, const std::map<std::string, Enum>& map, Enum defaultValue)
	{
		return token ? ToEnum(*token, map) : defaultValue;
	}

	template <typename... Accessors>
	static auto Parse(const Sentence& sentence, Accessors... accessors)
		-> std::tuple<decltype(accessors(sentence, 0))...>
	{
		return ParseSentencePart(sentence, 0, accessors...);
	}

private:
	template <typename Part>
	static std::size_t GetSentencePartSize(const Part& part)
	{
		return 1;
	}

	template <typename Part>
	static std::size_t GetSentencePartSize(const optional<Part>& optionalPart)
	{
		return optionalPart ? 1 : 0;
	}

	static std::tuple<> ParseSentencePart(const Sentence& sentence, std::size_t index)
	{
		if (sentence.GetNodes().size() > index)
			throw std::runtime_error("Sentence had more parts.");
		return{};
	}

	template <typename Accessor, typename... Accessors>
	static auto ParseSentencePart(const Sentence& sentence, std::size_t index, Accessor accessor, Accessors... accessors)
		-> std::tuple<decltype(accessor(sentence, 0)), decltype(accessors(sentence, 0))...>
	{
		auto part = accessor(sentence, index);
		auto nextIndex = index + GetSentencePartSize(part);
		auto parts = ParseSentencePart(sentence, nextIndex, accessors...);
		return std::tuple_cat(std::make_tuple(part), parts);
	}
};
