#pragma once
#include "SentenceParser.h"

//<function-statement> =
//	"function" id <parameter-clause-opt> <as-array-specifier-opt>
//	| "static" "function" id <parameter-clause-opt> <as-array-specifier-opt>
//	| "public" "function" id <parameter-clause-opt> <as-array-specifier-opt>
//	| "private" "function" id <parameter-clause-opt> <as-array-specifier-opt>
//	| "friend" "function" id <parameter-clause-opt> <as-array-specifier-opt>
//	| "public" "static" "function" id <parameter-clause-opt> <as-array-specifier-opt>
//	| "private" "static" "function" id <parameter-clause-opt> <as-array-specifier-opt>
//	| "friend" "static" "function" id <parameter-clause-opt> <as-array-specifier-opt>;
class VbFunctionStatement
{
public:
	VbFunctionStatement(const Sentence& sentence)
	{
		std::tie(access, isStatic, type, name, parameterClause, asArraySpecifier) =
			SentenceParser::Parse(
				sentence,
				OptionalToken("public", "private", "friend"),
				OptionalToken("static"),
				RequiredToken(),
				RequiredToken(),
				OptionalSentence("parameter-clause"),
				OptionalSentence("as-array-specifier"));
	}

	optional<Token> access;
	bool isStatic;
	Token type;
	Token name;
	optional<Sentence> parameterClause;
	optional<Sentence> asArraySpecifier;
};
