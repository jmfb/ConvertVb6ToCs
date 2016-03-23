#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

#include "VbTokenStream.h"
#include "VbConflictResolver.h"
#include "ParserLALR.h"

static const auto source = R"x(
	Public Function Foo( _
		ByVal index As Long, _
		ByRef found As Boolean _
	) As Variant
		If (index) Then
			found = True
		End If
	Done:
		Foo = CStr(index) : index = index + 1
	End Sub
)x";

void CompileGrammarToBinaryTransitionTable()
{
	std::cout << "Loading grammar text...";
	std::ifstream in{ R"(c:\save\code\tests\ConvertVb6ToCs\VbGrammar.txt)" };
	std::stringstream buffer;
	in >> buffer.rdbuf();
	std::cout << "done." << std::endl;

	std::cout << "Starting to load transition table" << std::endl;
	auto transitionTable = ParserLALR::Generate(buffer.str(), "translation-unit", VbConflictResolver{});
	std::cout << "Done loading transition table" << std::endl;

	std::cout << "Saving binary...";
	transitionTable.Write(BinaryWriter{ std::ofstream{ R"(c:\save\code\tests\ConvertVb6ToCs\VbGrammar.bin)", std::ios::binary } });
	std::cout << "done." << std::endl;
}

#include "VbTranslationUnit.h"
#include "VbTranslationHeader.h"
#include "VbModuleHeader.h"
#include "VbAttribute.h"
#include "VbDeclaration.h"
#include "VbLine.h"
#include "VbLineLabel.h"
#include "VbStatement.h"
#include "VbCompoundStatement.h"
#include "VbFunctionStatement.h"
#include "VbEndStatement.h"
#include "VbIfStatement.h"
#include "VbLetStatement.h"
#include "VbConstStatement.h"
#include "VbDeclareStatement.h"
#include "VbDefineTypeStatement.h"
#include "VbDimStatement.h"
#include "VbRedimStatement.h"
#include "VbPropertyStatement.h"
#include "VbTypeStatement.h"

#include "VbConstantDefinition.h"
#include "VbDimDefinition.h"
#include "VbRedimDefinition.h"
#include "VbTypeDefinition.h"

#include "VbAsSpecifier.h"
#include "VbSimpleType.h"
#include "VbTypeSpecifier.h"
#include "VbEndKeyword.h"
#include "VbSubscript.h"
#include "VbDefineTypeKeyword.h"
#include "VbArraySuffix.h"

#include "VbLiteral.h"
#include "VbQualifiedId.h"
#include "VbLValueList.h"
#include "VbLValue.h"
#include "VbLValueTerminal.h"

#include "VbExpression.h"
#include "VbXorExpression.h"
#include "VbOrExpression.h"
#include "VbAndExpression.h"
#include "VbMultiplicativeExpression.h"
#include "VbAdditiveExpression.h"
#include "VbEqualityExpression.h"
#include "VbRelationalExpression.h"
#include "VbUnaryExpression.h"
#include "VbPostfixExpression.h"
#include "VbPrimaryExpression.h"

void DumpStatement(const Sentence& sentence)
{
	VbStatement statement{ sentence };
	if (statement.functionStatement)
	{
		VbFunctionStatement functionStatement{ *statement.functionStatement };
		std::cout << "Function " << functionStatement.name.GetValue() << std::endl;
	}
	else if (statement.constStatement)
	{
		VbConstStatement constStatement{ *statement.constStatement };
		std::cout << "Const ";
		for (auto& definition : constStatement.constantDefinitions)
		{
			VbConstantDefinition constantDefinition{ definition };
			std::cout << constantDefinition.name.GetValue() << ", ";
		}
		std::cout << std::endl;
	}
	else if (statement.declareStatement)
	{
		VbDeclareStatement declareStatement{ *statement.declareStatement };
		std::cout << "Declare " << declareStatement.name.GetValue() << std::endl;
	}
	else if (statement.typeStatement)
	{
		VbTypeStatement typeStatement{ *statement.typeStatement };
		std::cout << "Type " << typeStatement.name.GetValue() << std::endl;
	}
	else if (statement.dimStatement)
	{
		VbDimStatement dimStatement{ *statement.dimStatement };
		std::cout << "Dim ";
		for (auto& definition : dimStatement.dimDefinitions)
		{
			VbDimDefinition dimDefinition{ definition };
			std::cout << dimDefinition.name.GetValue() << ", ";
		}
		std::cout << std::endl;
	}
	else if (statement.ifStatement)
	{
		VbIfStatement ifStatement{ *statement.ifStatement };
		std::cout << "If" << std::endl;
	}
	else if (statement.endStatement)
	{
		VbEndStatement end{ *statement.endStatement };
		std::cout << "End";
		if (end.keyword)
		{
			VbEndKeyword keyword{ *end.keyword };
			std::cout << " " << keyword.keyword.GetValue();
		}
		std::cout << std::endl;
	}
	else if (statement.letStatement)
	{
		VbLetStatement let{ *statement.letStatement };
		std::cout << "Let" << std::endl;
	}
}

void Dump(const Sentence& sentence)
{
	VbTranslationUnit translationUnit{ sentence };
	if (translationUnit.translationHeader)
	{
		VbTranslationHeader translationHeader{ *translationUnit.translationHeader };
		if (translationHeader.moduleHeader)
		{
			VbModuleHeader moduleHeader{ *translationHeader.moduleHeader };
			for (auto& attributeSentence : moduleHeader.attributes)
			{
				VbAttribute attribute{ attributeSentence };
				VbQualifiedId name{ attribute.name };
				std::cout << "Module Attribute: " << name.ToSimpleName() << std::endl;
				//TODO: expression
			}
		}
		//TODO: form/class header
	}
	if (!translationUnit.declarationList)
		return;
	for (auto& declarationSentence : *translationUnit.declarationList)
	{
		VbDeclaration declaration{ declarationSentence };
		//TODO: attribute
		if (declaration.lineLabel)
		{
			VbLineLabel lineLabel{ *declaration.lineLabel };
			std::cout << "Label: " << lineLabel.label.GetValue() << std::endl;
		}
		if (!declaration.vbLine)
			continue;
		VbLine vbLine{ *declaration.vbLine };
		if (vbLine.statement)
			DumpStatement(*vbLine.statement);
		if (!vbLine.compoundStatement)
			continue;
		VbCompoundStatement compoundStatement{ *vbLine.compoundStatement };
		for (auto& statement : compoundStatement.statements)
			DumpStatement(statement);
	}
}

void Process(const std::string& name, const std::string& source)
{
	std::cout << "Loading binary...";
	TransitionTable transitionTable;
	transitionTable.Read(BinaryReader{ std::ifstream{ R"(c:\save\code\tests\ConvertVb6ToCs\VbGrammar.bin)", std::ios::binary } });
	std::cout << "done." << std::endl;

	std::cout << "Parsing source...";
	auto sentence = transitionTable.Parse(VbTokenStream{ name, source });
	std::cout << "done." << std::endl;

	Dump(sentence);
}

int main(int argc, char** argv)
{
	try
	{
		//Process("source", source);

		auto fileName = R"(c:\save\code\tests\DSHECommon\basGlobals.bas)";
		std::ifstream in{ fileName };
		std::stringstream buffer;
		in >> buffer.rdbuf();
		Process(fileName, buffer.str());
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
	return 0;
}
