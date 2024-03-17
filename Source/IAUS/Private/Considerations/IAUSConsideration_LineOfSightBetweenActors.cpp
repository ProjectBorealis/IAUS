// Copyright Project Borealis

#include "Considerations/IAUSConsideration_LineOfSightBetweenActors.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "IAUSEvaluator.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IAUSConsideration_LineOfSightBetweenActors)

UIAUSConsideration_LineOfSightBetweenActors::UIAUSConsideration_LineOfSightBetweenActors()
{
	NodeName = TEXT("Line of Sight Between Actors");

	ActorAKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UIAUSConsideration_LineOfSightBetweenActors, ActorAKey), AActor::StaticClass());
	ActorBKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UIAUSConsideration_LineOfSightBetweenActors, ActorBKey), AActor::StaticClass());
}

FString UIAUSConsideration_LineOfSightBetweenActors::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %s and %s"), *Super::GetStaticDescription(), *ActorAKey.SelectedKeyName.ToString(), *ActorBKey.SelectedKeyName.ToString());
}

float UIAUSConsideration_LineOfSightBetweenActors::Score(const FIAUSBehaviorContext& Context) const
{
	if (!Context.AIController)
	{
		return 0;
	}
	
	const UBlackboardComponent* Blackboard = Context.AIController->GetBlackboardComponent();
	if (!Blackboard)
	{
		return 0.f;
	}
	
	const AActor* ActorA = Cast<AActor>(Blackboard->GetValueAsObject(ActorAKey.SelectedKeyName));
	const AActor* ActorB = Cast<AActor>(Blackboard->GetValueAsObject(ActorBKey.SelectedKeyName));
	if (!ActorA || !ActorB)
	{
		return 0.f;
	}
	
	const FVector& ActorALocation = ActorA->GetTargetLocation();
	const FVector& ActorBLocation = ActorB->GetTargetLocation();

	FCollisionQueryParams CollisionParams(SCENE_QUERY_STAT(LineOfSightBetweenActors), true);
	CollisionParams.AddIgnoredActor(ActorA);
	CollisionParams.AddIgnoredActor(ActorB);

	const bool bHit = GetWorld()->LineTraceTestByChannel(ActorALocation, ActorBLocation, ECC_Visibility, CollisionParams);
	return bHit ? 0.f : 1.f;
}
