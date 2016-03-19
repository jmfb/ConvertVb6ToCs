#pragma once

enum class PreTokenType
{
	EndOfFile,
	Whitespace,
	Newline,
	Comment,
	Identifier,
	Number,
	CharacterLiteral,
	StringLiteral,
	Operator,
	OtherChar
};
