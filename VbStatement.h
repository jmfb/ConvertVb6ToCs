#pragma once
#include "SentenceParser.h"

//<statement> =
//	#Option Statements
//	<option-explicit-statement>
//	| <option-base-statement>
//	| <option-compare-statement>
//	| <option-private-statement>
//	#If Statements
//	| <if-statement>
//	| <else-statement>
//	| <elseif-statement>
//	| <end-statement>
//	#Call and Let Statements
//	| <call-statement>
//	| <let-statement>
//	#Set Statements
//	| <set-statement>
//	| <lset-statement>
//	| <rset-statement>
//	#Local Function Statements
//	| <const-statement>
//	| <dim-statement>
//	| <with-statement>
//	#Array Statements
//	| <redim-statement>
//	| <erase-statement>
//	#Error Handling Statements
//	| <on-error-statement>
//	| <resume-statement>
//	| <error-statement>
//	| <stop-statement>
//	#Loop Statements (For, Do, While)
//	| <for-statement>
//	| <for-each-statement>
//	| <next-statement>
//	| <exit-statement>
//	| <do-statement>
//	| <loop-statement>
//	| <while-statement>
//	| <wend-statement>
//	#Control Statements (Select, GoSub, Goto)
//	| <select-statement>
//	| <case-statement>
//	| <gosub-statement>
//	| <return-statement>
//	| <goto-statement>
//	| <on-statement>
//	#Module Level Statements (Properties and Methods)
//	| <public-statement>
//	| <private-statement>
//	| <static-statement>
//	| <declare-statement>
//	| <sub-statement>
//	| <function-statement>
//	| <property-get-statement>
//	| <property-let-statement>
//	| <property-set-statement>
//	| <implements-statement>
//	#Enum and Type Statements
//	| <enum-statement>
//	| <type-statement>
//	#Global type definition (based on variable names)
//	| <define-type-statement>
//	#Event Statements
//	| <event-statement>
//	| <raise-event-statement>
//	#Misc. Statements (Mid(...) = ..., Randomize)
//	| <mid-statement>
//	#File I/O Statements
//	| <get-statement>
//	| <line-input-statement>
//	| <lock-statement>
//	| <unlock-statement>
//#<name-statement> causes the following ambiguity:
//#   Name(x) = 5     -> <l-value[<id.name> <l-value-suffix[(x)]>]> ...
//#   Name (x) As Y   -> <id.name> <expression[(x)]> ...
//#    | <name-statement>
//	| <open-statement>
//	| <put-statement>
//	| <seek-statement>
//	| <write-statement>
//;
class VbStatement
{
public:
	VbStatement(const Sentence& sentence)
		: VbStatement(SentenceParser::Parse(sentence,
			RequiredSentence()))
	{
	}

private:
	template <typename Tuple>
	VbStatement(const Tuple& result)
		: statement(std::get<0>(result))
	{
	}

public:
	Sentence statement;
};
