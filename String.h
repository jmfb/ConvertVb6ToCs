#pragma once
#include <regex>
#include <string>
#include <algorithm>
#include <cctype>

class String
{
public:
	static std::string ToLower(const std::string& value)
	{
		auto lower = value;
		std::transform(lower.begin(), lower.end(), lower.begin(), &std::tolower);
		return lower;
	}

	static bool Equals(const std::string& value1, const std::string& value2, bool caseSensitive)
	{
		return caseSensitive ?
			value1 == value2 :
			ToLower(value1) == ToLower(value2);
	}

	static bool EndsWith(const std::string& value, const std::string& suffix)
	{
		if (suffix.size() > value.size())
			return false;
		auto pos = value.rfind(suffix);
		return pos == (value.size() - suffix.size());
	}

	static std::string Replace(const std::string& source, const std::string& find, const std::string& replaceWith)
	{
		return std::regex_replace(source, std::regex{ find }, replaceWith);
	}

	static std::string EscapeXml(const std::string& value)
	{
		auto escaped = value;
		escaped = Replace(escaped, "&", "&amp;");
		escaped = Replace(escaped, "<", "&lt;");
		escaped = Replace(escaped, ">", "&gt;");
		return escaped;
	}
};
