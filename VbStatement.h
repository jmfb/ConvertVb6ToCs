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
	{
		std::tie(
			optionExplicitStatement,
			optionBaseStatement,
			optionCompareStatement,
			optionPrivateStatement,
			ifStatement,
			elseStatement,
			elseIfStatement,
			endStatement,
			callStatement,
			letStatement,
			setStatement,
			lsetStatement,
			rsetStatement,
			constStatement,
			dimStatement,
			withStatement,
			redimStatement,
			eraseStatement,
			onErrorStatement,
			resumeStatement,
			errorStatement,
			stopStatement,
			forStatement,
			forEachStatement,
			nextStatement,
			exitStatement,
			doStatement,
			loopStatement,
			whileStatement,
			wendStatement,
			selectStatement,
			caseStatement,
			gosubStatement,
			returnStatement,
			gotoStatement,
			onStatement,
			publicStatement,
			privateStatement,
			staticStatement,
			declareStatement,
			subStatement,
			functionStatement,
			propertyGetStatement,
			propertyLetStatement,
			propertySetStatement,
			implementsStatement,
			enumStatement,
			typeStatement,
			defineTypeStatement,
			eventStatement,
			raiseEventStatement,
			midStatement,
			getStatement,
			lineInputStatement,
			lockStatement,
			unlockStatement,
			openStatement,
			putStatement,
			seekStatement,
			writeStatement) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("option-explicit-statement"),
				OptionalSentence("option-base-statement"),
				OptionalSentence("option-compare-statement"),
				OptionalSentence("option-private-statement"),
				OptionalSentence("if-statement"),
				OptionalSentence("else-statement"),
				OptionalSentence("elseif-statement"),
				OptionalSentence("end-statement"),
				OptionalSentence("call-statement"),
				OptionalSentence("let-statement"),
				OptionalSentence("set-statement"),
				OptionalSentence("lset-statement"),
				OptionalSentence("rset-statement"),
				OptionalSentence("const-statement"),
				OptionalSentence("dim-statement"),
				OptionalSentence("with-statement"),
				OptionalSentence("redim-statement"),
				OptionalSentence("erase-statement"),
				OptionalSentence("on-error-statement"),
				OptionalSentence("resume-statement"),
				OptionalSentence("error-statement"),
				OptionalSentence("stop-statement"),
				OptionalSentence("for-statement"),
				OptionalSentence("for-each-statement"),
				OptionalSentence("next-statement"),
				OptionalSentence("exit-statement"),
				OptionalSentence("do-statement"),
				OptionalSentence("loop-statement"),
				OptionalSentence("while-statement"),
				OptionalSentence("wend-statement"),
				OptionalSentence("select-statement"),
				OptionalSentence("case-statement"),
				OptionalSentence("gosub-statement"),
				OptionalSentence("return-statement"),
				OptionalSentence("goto-statement"),
				OptionalSentence("on-statement"),
				OptionalSentence("public-statement"),
				OptionalSentence("private-statement"),
				OptionalSentence("static-statement"),
				OptionalSentence("declare-statement"),
				OptionalSentence("sub-statement"),
				OptionalSentence("function-statement"),
				OptionalSentence("property-get-statement"),
				OptionalSentence("property-let-statement"),
				OptionalSentence("property-set-statement"),
				OptionalSentence("implements-statement"),
				OptionalSentence("enum-statement"),
				OptionalSentence("type-statement"),
				OptionalSentence("define-type-statement"),
				OptionalSentence("event-statement"),
				OptionalSentence("raise-event-statement"),
				OptionalSentence("mid-statement"),
				OptionalSentence("get-statement"),
				OptionalSentence("line-input-statement"),
				OptionalSentence("lock-statement"),
				OptionalSentence("unlock-statement"),
				OptionalSentence("open-statement"),
				OptionalSentence("put-statement"),
				OptionalSentence("seek-statement"),
				OptionalSentence("write-statement"));
	}

	optional<Sentence> optionExplicitStatement;
	optional<Sentence> optionBaseStatement;
	optional<Sentence> optionCompareStatement;
	optional<Sentence> optionPrivateStatement;
	optional<Sentence> ifStatement;
	optional<Sentence> elseStatement;
	optional<Sentence> elseIfStatement;
	optional<Sentence> endStatement;
	optional<Sentence> callStatement;
	optional<Sentence> letStatement;
	optional<Sentence> setStatement;
	optional<Sentence> lsetStatement;
	optional<Sentence> rsetStatement;
	optional<Sentence> constStatement;
	optional<Sentence> dimStatement;
	optional<Sentence> withStatement;
	optional<Sentence> redimStatement;
	optional<Sentence> eraseStatement;
	optional<Sentence> onErrorStatement;
	optional<Sentence> resumeStatement;
	optional<Sentence> errorStatement;
	optional<Sentence> stopStatement;
	optional<Sentence> forStatement;
	optional<Sentence> forEachStatement;
	optional<Sentence> nextStatement;
	optional<Sentence> exitStatement;
	optional<Sentence> doStatement;
	optional<Sentence> loopStatement;
	optional<Sentence> whileStatement;
	optional<Sentence> wendStatement;
	optional<Sentence> selectStatement;
	optional<Sentence> caseStatement;
	optional<Sentence> gosubStatement;
	optional<Sentence> returnStatement;
	optional<Sentence> gotoStatement;
	optional<Sentence> onStatement;
	optional<Sentence> publicStatement;
	optional<Sentence> privateStatement;
	optional<Sentence> staticStatement;
	optional<Sentence> declareStatement;
	optional<Sentence> subStatement;
	optional<Sentence> functionStatement;
	optional<Sentence> propertyGetStatement;
	optional<Sentence> propertyLetStatement;
	optional<Sentence> propertySetStatement;
	optional<Sentence> implementsStatement;
	optional<Sentence> enumStatement;
	optional<Sentence> typeStatement;
	optional<Sentence> defineTypeStatement;
	optional<Sentence> eventStatement;
	optional<Sentence> raiseEventStatement;
	optional<Sentence> midStatement;
	optional<Sentence> getStatement;
	optional<Sentence> lineInputStatement;
	optional<Sentence> lockStatement;
	optional<Sentence> unlockStatement;
	optional<Sentence> openStatement;
	optional<Sentence> putStatement;
	optional<Sentence> seekStatement;
	optional<Sentence> writeStatement;
};
