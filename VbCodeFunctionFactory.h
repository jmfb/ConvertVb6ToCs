#pragma once
#include "VbCodeFunction.h"
#include "VbCodeFunctionType.h"
#include "VbCodeFunctionAccess.h"
#include "VbCodeStatement.h"
#include "Sentence.h"
#include "Token.h"
#include "optional.h"
#include <stack>

class VbCodeFunctionFactory
{
public:
	void AddStatement(const Sentence& sentence);
	bool IsInFunction() const;
	bool IsEndOfFunction() const;
	VbCodeFunction Create() const;

private:
	void ProcessStatement(const Sentence& sentence);
	void ProcessNextFunction(const Sentence& sentence);
	void BeginFunction(const Sentence& sentence);
	void BeginProperty(const Sentence& sentence);
	static VbCodeFunctionType ParseType(const Token& token);
	static VbCodeFunctionAccess ParseAccess(const optional<Token>& token);
	void ProcessEndStatement(const Sentence& sentence);
	void ProcessDimStatement(bool isStatic, const Sentence& sentence);
	void ProcessWithStatement(const Sentence& sentence);
	void ProcessLetStatement(const Sentence& sentence);
	void ProcessIfStatement(const Sentence& sentence);
	void ProcessElseIfStatement(const Sentence& sentence);
	void ProcessElseStatement(const Sentence& sentence);
	void ProcessCallStatement(const Sentence& sentence);

private:
	optional<VbCodeFunction> function;
	bool endOfFunction = false;
	std::stack<VbCodeStatementPtr> compoundStatements;
	std::stack<std::vector<VbCodeStatementPtr>*> blocks;
};
