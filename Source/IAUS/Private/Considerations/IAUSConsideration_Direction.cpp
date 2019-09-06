// Copyright 2019 Zachary Swoyer, Arran Walker

#include "IAUS/Public/Considerations/IAUSConsideration_Direction.h"

#include "IAUS/Public/Composites/IAUSBTComposite_Utility.h"

#include "AIController.h"

UIAUSConsideration_Direction::UIAUSConsideration_Direction()
{
}

float UIAUSConsideration_Direction::Score(const FIAUSBehaviorContext& Context) const
{
	FRotator TargetRotation = (Context.Actor->GetTargetLocation() - Context.AIController->GetPawn()->GetActorLocation()).Rotation();
	float Enddist = 1.0 - FMath::Clamp<float>(FVector::DotProduct(TargetRotation.Vector(), Context.AIController->GetPawn()->GetActorRotation().Vector()), 0.0, 1.0);

	return ResponseCurve->ComputeValue(Enddist);
}