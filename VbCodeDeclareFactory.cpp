#include "VbCodeDeclareFactory.h"
#include "VbDeclareStatement.h"
#include "VbDeclareAlias.h"
#include "VbCodeParameterFactory.h"
#include "VbCodeTypeFactory.h"
#include "VbCodeValueFactory.h"

VbCodeDeclare VbCodeDeclareFactory::Create(const Sentence& sentence)
{
	VbDeclareStatement declareStatement{ sentence };
	return
	{
		declareStatement.access && *declareStatement.access == "public",
		declareStatement.name.GetValue(),
		declareStatement.declareAlias ? VbCodeValueFactory::Create(VbDeclareAlias{ *declareStatement.declareAlias }.alias).stringValue : "",
		VbCodeValueFactory::Create(declareStatement.library).stringValue,
		VbCodeParameterFactory::Create(declareStatement.parameterClause),
		VbCodeTypeFactory::CreateOptional(declareStatement.asArraySpecifier)
	};
}
