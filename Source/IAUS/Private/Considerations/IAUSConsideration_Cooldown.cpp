// Copyright Project Borealis

#include "Considerations/IAUSConsideration_Cooldown.h"

#include "Engine/World.h"

#include "IAUSEvaluator.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IAUSConsideration_Cooldown)

UIAUSConsideration_Cooldown::UIAUSConsideration_Cooldown()
{
	CoolDownTime = 5.0f;
}

float UIAUSConsideration_Cooldown::Score(const FIAUSBehaviorContext& Context) const
{
	if (Node.IsNone())
	{
		return 0.0f;
	}

	const FString& Name = Node.ToString();
	if (!Context.Evaluator->BehaviorNameIndexMap.Contains(Name))
	{
		return 0.0f;
	}

	const int32 Index = Context.Evaluator->BehaviorNameIndexMap[Name];

	if (Context.Evaluator->Behaviors[Index].LastExecutionTime == 0.0f)
	{
		return ResponseCurve->ComputeValue(1.0f);
	}

	const float Seconds = GetWorld()->GetTimeSeconds() - Context.Evaluator->Behaviors[Index].LastExecutionTime;

	return ResponseCurve->ComputeValue(Seconds / CoolDownTime);
}
