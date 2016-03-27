#include "VbCodeValueFactory.h"

VbCodeValue VbCodeValueFactory::Create(const Token& token)
{
	if (token.GetType() == TokenType::Keyword)
	{
		if (token == "true")
			return{ true };
		if (token == "false")
			return{ false };
		throw std::runtime_error("Not implemented: Other Literal keywords");
	}
	auto& literal = token.GetValue();
	if (literal.empty())
		throw std::runtime_error("Empty literal is not valid.");
	if (literal[0] != '"')
		throw std::runtime_error("Not implemented: non-string literals.");
	if (literal.back() != '"')
		throw std::runtime_error("String must end in quote.");
	return{ literal.substr(1, literal.size() - 2) };
}
