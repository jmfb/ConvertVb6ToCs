#include "VbCodeParameterFactory.h"
#include "VbParameterClause.h"
#include "VbParameter.h"
#include "VbPassBy.h"
#include "VbCodeTypeFactory.h"
#include "VbCodeExpressionFactory.h"

std::vector<VbCodeParameter> VbCodeParameterFactory::Create(const optional<Sentence>& sentence)
{
	if (sentence)
		return Create(*sentence);
	return{};
}

std::vector<VbCodeParameter> VbCodeParameterFactory::Create(const Sentence& sentence)
{
	VbParameterClause parameterClause{ sentence };
	if (!parameterClause.parameters)
		return{};
	std::vector<VbCodeParameter> parameters;
	for (auto& parameterSentence : *parameterClause.parameters)
	{
		VbParameter parameter{ parameterSentence };
		parameters.emplace_back(
			parameter.passBy && VbPassBy{ *parameter.passBy }.passBy == "byval",
			parameter.name.GetValue(),
			VbCodeTypeFactory::Create(parameter.asSpecifier),
			parameter.isOptional,
			parameter.isParamArray,
			VbCodeExpressionFactory::CreateDefaultValue(parameter.defaultValue));
	}
	return parameters;
}
