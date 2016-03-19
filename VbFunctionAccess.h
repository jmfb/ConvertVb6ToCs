#pragma once
#include <stdexcept>

enum class VbFunctionAccess
{
	Public,
	Private,
	Friend
};

inline const char* ToString(VbFunctionAccess value)
{
	switch (value)
	{
	case VbFunctionAccess::Public: return "Public";
	case VbFunctionAccess::Private: return "Private";
	case VbFunctionAccess::Friend: return "Friend";
	}
	throw std::runtime_error("Invalid function access.");
}
