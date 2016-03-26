#pragma once
#include "Sentence.h"
#include "VbCodeExpression.h"

class VbCodeExpressionFactory
{
public:
	static VbCodeExpressionPtr CreateExpression(const Sentence& sentence);
	static VbCodeExpressionPtr CreateOrExpression(const Sentence& sentence);
	static VbCodeExpressionPtr CreateXorExpression(const Sentence& sentence);
	static VbCodeExpressionPtr CreateAndExpression(const Sentence& sentence);
	static VbCodeExpressionPtr CreateEqualityExpression(const Sentence& sentence);
	static VbCodeExpressionPtr CreateRelationalExpression(const Sentence& sentence);
	static VbCodeExpressionPtr CreateAdditiveExpression(const Sentence& sentence);
	static VbCodeExpressionPtr CreateMultiplicativeExpression(const Sentence& sentence);
	static VbCodeExpressionPtr CreateUnaryExpression(const Sentence& sentence);
	static VbCodeExpressionPtr CreatePostfixExpression(const Sentence& sentence);
	static VbCodeExpressionPtr CreatePrimaryExpression(const Sentence& sentence);
	static VbCodeExpressionPtr CreateConstantExpression(const Sentence& sentence);
};