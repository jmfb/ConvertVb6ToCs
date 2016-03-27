#include "VbCodeClassFactory.h"
#include "VbTranslationUnit.h"
#include "VbTranslationHeader.h"
#include "VbClassHeader.h"
#include "VbSettingBlock.h"
#include "VbSetting.h"
#include "VbSettingValue.h"
#include "VbAttribute.h"
#include "VbLValue.h"
#include "VbQualifiedId.h"
#include "VbCodeExpressionFactory.h"

VbCodeClass VbCodeClassFactory::Create(const std::string& library, const Sentence& sentence)
{
	VbTranslationUnit translationUnit{ sentence };
	if (!translationUnit.translationHeader)
		throw std::runtime_error("Missing translation header.");
	VbTranslationHeader translationHeader{ *translationUnit.translationHeader };
	if (!translationHeader.classHeader)
		throw std::runtime_error("Missing class header.");
	LoadHeader(*translationHeader.classHeader);
	return
	{
		library,
		name
	};
}

void VbCodeClassFactory::LoadHeader(const Sentence& sentence)
{
	VbClassHeader classHeader{ sentence };
	VbSettingBlock settingBlock{ classHeader.settingBlock };
	if (settingBlock.settings)
		for (auto& settingSentence : *settingBlock.settings)
		{
			VbSetting setting{ settingSentence };
			VbLValue lValue{ setting.lValue };
			VbSettingValue settingValue{ setting.settingValue };
			//TODO: MultiUse, Persistable, DataBindingBehavior, DataSourceBehavior, MTSTransactionMode
		}
	for (auto& attributeSentence : classHeader.attributes)
	{
		VbAttribute attribute{ attributeSentence };
		auto attributeName = VbQualifiedId{ attribute.name }.ToSimpleName();
		auto value = VbCodeExpressionFactory::CreateExpression(attribute.value)->EvaluateConstant();
		if (attributeName == "VB_Name")
		{
			if (value.type != VbCodeValueType::String)
				throw std::runtime_error("VB_Name should be a string.");
			name = value.stringValue;
		}
		//TODO: VB_GlobalNameSpace, VB_Creatable, VB_PredeclaredId, VB_Exposed
	}
}
