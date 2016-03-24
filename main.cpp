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
#include "VbQualifiedId.h"

#if 0
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
#include "VbWithStatement.h"
#include "VbElseIfStatement.h"
#include "VbElseStatement.h"
#include "VbCallStatement.h"

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
#include "VbLValueList.h"
#include "VbLValue.h"
#include "VbLValueTerminal.h"
#include "VbCallSuffix.h"
#include "VbCallTerminal.h"
#endif

#if 0
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
	else if (statement.publicStatement)
	{
		VbDimStatement dimStatement{ *statement.publicStatement };
		std::cout << "Public ";
		for (auto& definition : dimStatement.dimDefinitions)
		{
			VbDimDefinition dimDefinition{ definition };
			std::cout << dimDefinition.name.GetValue() << ", ";
		}
		std::cout << std::endl;
	}
	else if (statement.staticStatement)
	{
		VbDimStatement dimStatement{ *statement.staticStatement };
		std::cout << "Static ";
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
	else if (statement.elseIfStatement)
	{
		VbElseIfStatement elseIfStatement{ *statement.elseIfStatement };
		std::cout << "ElseIf" << std::endl;
	}
	else if (statement.elseStatement)
	{
		VbElseStatement elseStatement{ *statement.elseStatement };
		std::cout << "Else" << std::endl;
	}
	else if (statement.withStatement)
	{
		VbWithStatement withStatement{ *statement.withStatement };
		std::cout << "With" << std::endl;
	}
	else if (statement.endStatement)
	{
		VbEndStatement endStatement{ *statement.endStatement };
		std::cout << "End";
		if (endStatement.keyword)
		{
			VbEndKeyword keyword{ *endStatement.keyword };
			std::cout << " " << keyword.keyword.GetValue();
		}
		std::cout << std::endl;
	}
	else if (statement.letStatement)
	{
		VbLetStatement letStatement{ *statement.letStatement };
		std::cout << "Let" << std::endl;
	}
	else if (statement.callStatement)
	{
		VbCallStatement callStatement{ *statement.callStatement };
		std::cout << "Call";
		//TODO: lValue variant...
		if (callStatement.callSuffix)
		{
			VbCallSuffix callSuffix{ *callStatement.callSuffix };
			VbCallTerminal callTerminal{ callSuffix.callTerminal };
			std::cout << " " << callTerminal.name.GetValue();
			for (auto& suffix : callSuffix.suffix)
			{
				//TODO: dot type
				VbCallTerminal nextCallTerminal{ suffix.second };
				std::cout << "." << nextCallTerminal.name.GetValue();
			}
		}
		std::cout << std::endl;
	}
}
#endif

#include "VbCodeExpressionFactory.h"

class VbCodeModule
{
public:
	VbCodeModule(const Sentence& sentence)
	{
		VbTranslationUnit translationUnit{ sentence };
		LoadHeader(translationUnit);
		//TODO
	}

private:
	void LoadHeader(const VbTranslationUnit& translationUnit)
	{
		if (!translationUnit.translationHeader)
			throw std::runtime_error("Missing translation header.");
		VbTranslationHeader translationHeader{ *translationUnit.translationHeader };
		if (!translationHeader.moduleHeader)
			throw std::runtime_error("Missing module header.");
		VbModuleHeader moduleHeader{ *translationHeader.moduleHeader };
		if (moduleHeader.attributes.size() != 1)
			throw std::runtime_error("Should be exactly one attribute.");
		VbAttribute attribute{ moduleHeader.attributes[0] };
		auto attributeName = VbQualifiedId{ attribute.name }.ToSimpleName();
		if (attributeName != "VB_Name")
			throw std::runtime_error("Only supported module attribute is VB_Name");
		auto value = VbCodeExpressionFactory::CreateExpression(attribute.value)->EvaluateConstant();
		if (value.type != VbCodeValueType::String)
			throw std::runtime_error("VB_Name should be a string.");
		name = value.stringValue;
	}

public:
	std::string name;
};

#if 0
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
#endif

void Process(const std::string& name, const std::string& source)
{
	TransitionTable transitionTable;
	transitionTable.Read(BinaryReader{ std::ifstream{ R"(c:\save\code\tests\ConvertVb6ToCs\VbGrammar.bin)", std::ios::binary } });
	auto sentence = transitionTable.Parse(VbTokenStream{ name, source });
	sentence.WriteXml(std::ofstream{ R"(c:\temp\output.xml)" });

	VbCodeModule module{ sentence };
	std::cout << module.name << std::endl;
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
