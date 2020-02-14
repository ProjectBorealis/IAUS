// Copyright 2017-2020 Project Borealis

#include "IAUS/Public/Considerations/IAUSConsideration_Health.h"

#include "AIController.h"

#include "Components/IAUSBaseHealthComponent.h"
#include "IAUSEvaluator.h"

UIAUSConsideration_Health::UIAUSConsideration_Health()
{
	TargetsSelf = true;
}

float UIAUSConsideration_Health::Score(const FIAUSBehaviorContext& Context) const
{
	AActor* Actor = Context.Actor;
	if (TargetsSelf)
	{
		Actor = Context.AIController->GetPawn();
	}

	const UIAUSBaseHealthComponent* HealthComponent = Actor->FindComponentByClass<UIAUSBaseHealthComponent>();

	if (!HealthComponent || HealthComponent->GetMaxHealth() <= 0.0f)
	{
		return 1.0f;
	}

	return ResponseCurve->ComputeValue(HealthComponent->GetHealth() / HealthComponent->GetMaxHealth());
}
