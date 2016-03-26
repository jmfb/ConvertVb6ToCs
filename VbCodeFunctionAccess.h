#pragma once
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
