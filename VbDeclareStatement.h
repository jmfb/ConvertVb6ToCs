#pragma once
#include "SentenceParser.h"

//<declare-statement> =
//	<declare-sub-statement>
//	| <declare-function-statement>;
//<declare-sub-statement> =
//	"public" "declare" "sub" id <id.lib> val <declare-alias-opt> <parameter-clause-opt>
//	| "private" "declare" "sub" id <id.lib> val <declare-alias-opt> <parameter-clause-opt>
//	| "declare" "sub" id <id.lib> val <declare-alias-opt> <parameter-clause-opt>;
//<declare-function-statement> =
//	"public" "declare" "function" id <id.lib> val <declare-alias-opt> <parameter-clause-opt> <as-array-specifier-opt>
//	| "private" "declare" "function" id <id.lib> val <declare-alias-opt> <parameter-clause-opt> <as-array-specifier-opt>
//	| "declare" "function" id <id.lib> val <declare-alias-opt> <parameter-clause-opt> <as-array-specifier-opt>;
class VbDeclareStatement
{
public:
	VbDeclareStatement(const Sentence& sentence)
	{
		Sentence innerSentence;
		std::tie(innerSentence) = SentenceParser::Parse(
			sentence,
			RequiredSentence("declare-sub-statement", "declare-function-statement"));
		std::tie(access, std::ignore, type, name, std::ignore, library, declareAlias, parameterClause, asArraySpecifier) =
			SentenceParser::Parse(
				innerSentence,
				OptionalToken("public", "private"),
				RequiredToken("declare"),
				RequiredToken("sub", "function"),
				RequiredToken(),
				RequiredToken("lib"),
				RequiredToken(),
				OptionalSentence("declare-alias"),
				OptionalSentence("parameter-clause"),
				OptionalSentence("as-array-specifier"));
	}

	optional<Token> access;
	Token type;
	Token name;
	Token library;
	optional<Sentence> declareAlias;
	optional<Sentence> parameterClause;
	optional<Sentence> asArraySpecifier;
};
