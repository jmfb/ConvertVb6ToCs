#pragma once
#include "SentenceParser.h"

//<define-type-keyword> =
//	"defbool"
//	| "defbyte"
//	| "defint"
//	| "deflng"
//	| "defcur"
//	| "defsng"
//	| "defdbl"
//	| "defdate"
//	| "defstr"
//	| "defobj"
//	| "defvar";
class VbDefineTypeKeyword
{
public:
	VbDefineTypeKeyword(const Sentence& sentence)
	{
		std::tie(keyword) = SentenceParser::Parse(
			sentence,
			RequiredToken(
				"defbool",
				"defbyte",
				"defint",
				"deflng",
				"defcur",
				"defsng",
				"defdbl",
				"defdate",
				"defstr",
				"defobj",
				"defvar"));
	}

	Token keyword;
};
