#pragma once
#include "EnumParser.h"
#include <stdexcept>

enum class VbCodeFunctionAccess
{
	Public,
	Private,
	Friend
};

inline const char* ToString(VbCodeFunctionAccess value)
{
	switch (value)
	{
	case VbCodeFunctionAccess::Public: return "Public";
	case VbCodeFunctionAccess::Private: return "Private";
	case VbCodeFunctionAccess::Friend: return "Friend";
	}
	throw std::runtime_error("Invalid function access.");
}

inline const char* ToCs(VbCodeFunctionAccess value)
{
	switch (value)
	{
	case VbCodeFunctionAccess::Public: return "public";
	case VbCodeFunctionAccess::Private: return "private";
	case VbCodeFunctionAccess::Friend: return "internal";
	}
	throw std::runtime_error("Invalid function access.");
}

inline VbCodeFunctionAccess ToFunctionAccess(const optional<Token>& token)
{
	return EnumParser::Parse(
		token,
		{
			{ "public", VbCodeFunctionAccess::Public },
			{ "private", VbCodeFunctionAccess::Private },
			{ "friend", VbCodeFunctionAccess::Friend }
		},
		VbCodeFunctionAccess::Private);
}
