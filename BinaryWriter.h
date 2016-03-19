#pragma once
#include <iostream>
#include <string>

class BinaryWriter
{
public:
	BinaryWriter(std::ostream& out)
		: out(out)
	{
	}

	void Write(std::size_t value)
	{
		Write(&value, sizeof(value));
	}

	void Write(const std::string& value)
	{
		Write(value.size());
		Write(value.c_str(), value.size());
	}

private:
	void Write(const void* data, std::size_t bytes)
	{
		out.write(reinterpret_cast<const char*>(data), bytes);
	}

private:
	std::ostream& out;
};
