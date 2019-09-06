// Copyright 2017-2018 Arran Walker

#include "IAUS/Public/Considerations/IAUSConsideration_Health.h"

#include "AIController.h"

#include "Components/IAUSHealthComponent.h"
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

	const UIAUSHealthComponent* HealthComponent = Cast<UIAUSHealthComponent>(Actor->GetComponentByClass(UIAUSHealthComponent::StaticClass()));

	if (!HealthComponent)
	{
		return 0;
	}

	return ResponseCurve->ComputeValue(HealthComponent->GetHealth() / HealthComponent->GetMaxHealth());
}
