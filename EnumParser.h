#pragma once
#include "Token.h"
#include "optional.h"
#include <string>
#include <map>
#include <stdexcept>

class EnumParser
{
public:
	template <typename Enum>
	static Enum Parse(const Token& token, const std::map<std::string, Enum>& map)
	{
		for (auto& entry : map)
			if (token == entry.first)
				return entry.second;
		throw std::runtime_error("Invalid enum value.");
	}

	template <typename Enum>
	static Enum Parse(const optional<Token>& token, const std::map<std::string, Enum>& map, Enum defaultValue)
	{
		return token ? Parse(*token, map) : defaultValue;
	}
};
