#pragma once
#include "Char.h"
#include <string>
#include <stack>
#include <regex>

class CharStream
{
public:
	CharStream(const std::string& name, const std::string& value)
		: name(name), value(value)
	{
		CorrectNewlines();
	}

	const std::string& GetName() const
	{
		return name;
	}

	bool IsEndOfFile() const
	{
		return buffer.empty() && index >= value.size();
	}

	void MoveFirst()
	{
		index = 0;
		line = 1;
		column = 1;
		buffer = {};
	}

	void PutBack(const Char& ch)
	{
		buffer.push(ch);
	}

	Char GetNext()
	{
		if (buffer.empty())
			return InternalGetNext();
		auto result = buffer.top();
		buffer.pop();
		return result;
	}

	Char ScanPast(const std::string& str)
	{
		auto retval = Char::EndOfFile();

		std::stack<Char> scanned;
		while (!IsEndOfFile())
		{
			auto ch = GetNext();
			scanned.push(ch);
			if (str.find(ch.GetValue()) != std::string::npos)
				continue;
			retval = ch;
			break;
		}

		while (!scanned.empty())
		{
			PutBack(scanned.top());
			scanned.pop();
		}

		return retval;
	}

private:
	void CorrectNewlines()
	{
		value = std::regex_replace(value, std::regex{ R"(\r\n)" }, "\n");
		value = std::regex_replace(value, std::regex{ R"(\r)" }, "\n");
		if (value.empty() || value.back() != '\n')
			value += '\n';
	}

protected:
	virtual Char InternalGetNext() = 0;

protected:
	std::string name;
	std::string value;
	std::size_t index = 0;
	int line = 1;
	int column = 1;
	std::stack<Char> buffer;
};
