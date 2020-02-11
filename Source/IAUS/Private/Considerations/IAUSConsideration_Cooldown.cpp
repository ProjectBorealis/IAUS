// Copyright 2017-2020 Project Borealis

#include "IAUS/Public/Considerations/IAUSConsideration_Cooldown.h"

#include "IAUS/Public/Composites/IAUSBTComposite_Utility.h"

#include "AIController.h"
#include "Engine/World.h"

UIAUSConsideration_Cooldown::UIAUSConsideration_Cooldown()
{
	CoolDownTime = 5.0f;
}

float UIAUSConsideration_Cooldown::Score(const FIAUSBehaviorContext& Context) const
{
	FString Name = Node.ToString();
	if (Node == NAME_None || Name.Len() == 0)
	{
		return 0;
	}

	if (!Context.Evaluator->BehaviorNameIndex.Contains(Name))
	{
		return 0;
	}

	int32 Index = Context.Evaluator->BehaviorNameIndex[Node.ToString()];

	if (Context.Evaluator->Behaviors[Index].LastExecutionTime == 0)
	{
		return ResponseCurve->ComputeValue(1);
	}

	float Seconds = GetWorld()->GetTimeSeconds() - Context.Evaluator->Behaviors[Index].LastExecutionTime;

	return ResponseCurve->ComputeValue(Seconds / CoolDownTime);
}
