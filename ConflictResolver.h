#pragma once
#include "ConflictResolution.h"
#include "Terminal.h"
#include "TransitionAction.h"
#include "TransitionType.h"

class ConflictResolver
{
public:
	virtual ConflictResolution Resolve(Terminal terminal, TransitionAction OldAction, TransitionAction NewAction) = 0;

protected:
	static ConflictResolution ChooseResolution(TransitionType oldTransition, TransitionType newTransition, TransitionType keepTransition)
	{
		if (oldTransition == newTransition)
			return ConflictResolution::None;
		if (oldTransition == keepTransition)
			return ConflictResolution::KeepOld;
		if (newTransition == keepTransition)
			return ConflictResolution::KeepNew;
		return ConflictResolution::None;
	}
};
