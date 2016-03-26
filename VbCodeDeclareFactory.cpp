#include "VbCodeDeclareFactory.h"
#include "VbDeclareStatement.h"
#include "VbDeclareAlias.h"
#include "VbCodeParameterFactory.h"
#include "VbCodeTypeFactory.h"

VbCodeDeclare VbCodeDeclareFactory::Create(const Sentence& sentence)
{
	VbDeclareStatement declareStatement{ sentence };
	return
	{
		declareStatement.access && *declareStatement.access == "public",
		declareStatement.name.GetValue(),
		declareStatement.declareAlias ? VbDeclareAlias{ *declareStatement.declareAlias }.alias.GetValue() : "",
		declareStatement.library.GetValue(),
		VbCodeParameterFactory::Create(declareStatement.parameterClause),
		VbCodeTypeFactory::CreateOptional(declareStatement.asArraySpecifier)
	};
}
