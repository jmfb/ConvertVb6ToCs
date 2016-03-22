#pragma once
#include "SentenceParser.h"
#include "VbFunctionAccess.h"
#include "VbFunctionType.h"

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
		optional<Token> accessToken;
		optional<Token> staticToken;
		Token typeToken;
		Token nameToken;
		std::tie(accessToken, staticToken, typeToken, nameToken, parameterClause, asArraySpecifier) =
			SentenceParser::Parse(
				sentence,
				OptionalToken("public", "private", "friend"),
				OptionalToken("static"),
				RequiredToken(),
				RequiredToken(),
				OptionalSentence("parameter-clause"),
				OptionalSentence("as-array-specifier"));
		access = ParseAccess(accessToken);
		isStatic = staticToken;
		type = ParseType(typeToken);
		name = nameToken.GetValue();
	}

	static VbFunctionAccess ParseAccess(const optional<Token>& token)
	{
		return SentenceParser::ToEnum(
			token,
			{
				{ "public", VbFunctionAccess::Public },
				{ "private", VbFunctionAccess::Private },
				{ "friend", VbFunctionAccess::Friend }
			},
			VbFunctionAccess::Public);
	}
	static VbFunctionType ParseType(const Token& token)
	{
		return SentenceParser::ToEnum<VbFunctionType>(
			token,
			{
				{ "sub", VbFunctionType::Sub },
				{ "function", VbFunctionType::Function }
			});
	}

	VbFunctionAccess access;
	bool isStatic;
	VbFunctionType type;
	std::string name;
	optional<Sentence> parameterClause;
	optional<Sentence> asArraySpecifier;
};
