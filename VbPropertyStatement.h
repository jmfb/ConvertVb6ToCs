#pragma once
#include "SentenceParser.h"

//<property-get-statement> =
//	<id.property> "get" id <parameter-clause-opt> <as-array-specifier-opt>
//	| "static" <id.property> "get" id <parameter-clause-opt> <as-array-specifier-opt>
//	| "public" <id.property> "get" id <parameter-clause-opt> <as-array-specifier-opt>
//	| "private" <id.property> "get" id <parameter-clause-opt> <as-array-specifier-opt>
//	| "friend" <id.property> "get" id <parameter-clause-opt> <as-array-specifier-opt>
//	| "public" "static" <id.property> "get" id <parameter-clause-opt> <as-array-specifier-opt>
//	| "private" "static" <id.property> "get" id <parameter-clause-opt> <as-array-specifier-opt>
//	| "friend" "static" <id.property> "get" id <parameter-clause-opt> <as-array-specifier-opt>;
//<property-let-statement> =
//	<id.property> "let" id <parameter-clause>
//	| "static" <id.property> "let" id <parameter-clause>
//	| "public" <id.property> "let" id <parameter-clause>
//	| "private" <id.property> "let" id <parameter-clause>
//	| "friend" <id.property> "let" id <parameter-clause>
//	| "public" "static" <id.property> "let" id <parameter-clause>
//	| "private" "static" <id.property> "let" id <parameter-clause>
//	| "friend" "static" <id.property> "let" id <parameter-clause>;
//<property-set-statement> =
//	<id.property> "set" id <parameter-clause>
//	| "static" <id.property> "set" id <parameter-clause>
//	| "public" <id.property> "set" id <parameter-clause>
//	| "private" <id.property> "set" id <parameter-clause>
//	| "friend" <id.property> "set" id <parameter-clause>
//	| "public" "static" <id.property> "set" id <parameter-clause>
//	| "private" "static" <id.property> "set" id <parameter-clause>
//	| "friend" "static" <id.property> "set" id <parameter-clause>;
class VbPropertyStatement
{
public:
	VbPropertyStatement(const Sentence& sentence)
	{
		std::tie(access, isStatic, std::ignore, type, name, parameterClause, asArraySpecifier) =
			SentenceParser::Parse(
				sentence,
				OptionalToken("public", "private", "friend"),
				OptionalToken("static"),
				RequiredToken("property"),
				RequiredToken("get", "let", "set"),
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
