#pragma once
#include <string>
#include <sstream>

class Position
{
public:
	Position() = default;
	Position(const std::string& source, int line, int column)
		: source(source), line(line), column(column)
	{
	}
	Position(const Position& rhs) = default;
	~Position() = default;

	Position& operator=(const Position& rhs) = default;

	const std::string& GetSource() const
	{
		return source;
	}
	int GetLine() const
	{
		return line;
	}
	int GetColumn() const
	{
		return column;
	}

	std::string ToString() const
	{
		std::ostringstream out;
		out << source << "(Line " << line << ", Col " << column << ")";
		return out.str();
	}

private:
	std::string source;
	int line = 0;
	int column = 0;
};
