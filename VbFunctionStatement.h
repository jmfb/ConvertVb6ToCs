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
class FunctionStatement
{
public:
	FunctionStatement(const Sentence& sentence)
		: FunctionStatement(SentenceParser::Parse(sentence,
			OptionalToken("public", "private", "friend"),
			OptionalToken("static"),
			RequiredToken(),
			RequiredToken(),
			OptionalSentence("parameter-clause"),
			OptionalSentence("as-array-specifier")))
	{
	}

private:
	template <typename Tuple>
	FunctionStatement(const Tuple& result)
		: access(ParseAccess(std::get<0>(result))),
		isStatic(std::get<1>(result)),
		type(ParseType(std::get<2>(result))),
		name(std::get<3>(result).GetValue())
	{
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

public:
	VbFunctionAccess access;
	bool isStatic;
	VbFunctionType type;
	std::string name;
	//TODO: return type
	//TODO: parameters
};
