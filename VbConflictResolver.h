#pragma once
#include "ConflictResolver.h"

class VbConflictResolver : public ConflictResolver
{
public:
	ConflictResolution Resolve(Terminal terminal, TransitionAction OldAction, TransitionAction NewAction) final
	{
		//To resolve the dangling else problem, match the else with the innermost if (shift instead of reduce)
		if (terminal.GetType() == TokenType::Keyword && terminal.GetValue() == "else")
			return ChooseResolution(OldAction.GetType(), NewAction.GetType(), TransitionType::Shift);
		//Resolve id ':' ambiguity as <line-label> instead of <call-statement> ':' by picking "shift" operation
		else if (terminal.GetType() == TokenType::Operator && terminal.GetValue() == ":")
			return ChooseResolution(OldAction.GetType(), NewAction.GetType(), TransitionType::Shift);
		//Resolve <id.Attribute> ambiguity by shifting as much into header as possible.
		else if (terminal.GetType() == TokenType::Identifier && terminal.GetValue() == "attribute")
			return ChooseResolution(OldAction.GetType(), NewAction.GetType(), TransitionType::Shift);
		//Unrecognized ambiguity: cannot resolve this.
		return ConflictResolution::None;
	}
};
