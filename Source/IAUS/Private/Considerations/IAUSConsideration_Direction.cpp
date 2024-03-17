// Copyright Project Borealis

#include "Considerations/IAUSConsideration_Direction.h"

#include "AIController.h"

#include "IAUSEvaluator.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IAUSConsideration_Direction)

UIAUSConsideration_Direction::UIAUSConsideration_Direction() {}

float UIAUSConsideration_Direction::Score(const FIAUSBehaviorContext& Context) const
{
	if (!Context.Target || !Context.AIController || !Context.AIController->GetPawn())
	{
		// return a result like if the dot product result was zero
		return ResponseCurve->ComputeValue(1.f);
	}
	
	const FVector DirectionToTarget = Context.Target->GetTargetLocation() - Context.AIController->GetPawn()->GetActorLocation();
	float Enddist = 1.0 - FMath::Clamp<float>(FVector::DotProduct(DirectionToTarget, Context.AIController->GetPawn()->GetActorForwardVector()), 0.0, 1.0);

	return ResponseCurve->ComputeValue(Enddist);
}
