#pragma once
#include <iostream>
#include <string>
#include <vector>

class BinaryReader
{
public:
	BinaryReader(std::istream& in)
		: in(in)
	{
	}

	std::size_t ReadSize()
	{
		std::size_t result = 0;
		Read(&result, sizeof(result));
		return result;
	}

	std::string ReadString()
	{
		std::vector<char> buffer(ReadSize());
		Read(buffer.data(), buffer.size());
		return{ buffer.data(), buffer.size() };
	}

private:
	void Read(void* data, std::size_t bytes)
	{
		in.read(reinterpret_cast<char*>(data), bytes);
	}

private:
	std::istream& in;
};
