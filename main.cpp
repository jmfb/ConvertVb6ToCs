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
		Foo = CStr(index)
	End Sub
)x";

int main(int argc, char** argv)
{
	try
	{
		std::cout << "Loading grammar text...";
		std::ifstream in{ R"(c:\save\code\tests\ConvertVb6ToCs\VbGrammar.txt)" };
		std::stringstream buffer;
		in >> buffer.rdbuf();
		std::cout << "done." << std::endl;

		std::cout << "Starting to load transition table" << std::endl;
		auto transitionTable = ParserLALR::Generate(buffer.str(), "translation-unit", VbConflictResolver{});
		std::cout << "Done loading transition table" << std::endl;
		
		std::cout << "Parsing source...";
		auto sentence = transitionTable.Parse(VbTokenStream{ "source", source });
		std::cout << "done." << std::endl;

		std::cout << "Saving to xml...";
		std::ofstream out{ R"(c:\temp\output.xml)" };
		sentence.WriteXml(out);
		std::cout << "done." << std::endl;

		std::cout << "Success" << std::endl;
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
	return 0;
}
