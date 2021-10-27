// Copyright Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "Decorators/IAUSBTDecorator_Consideration.h"

#include "IAUSConsideration_LineOfSightBetweenActors.generated.h"

/**
 *
 */
UCLASS(Meta = (DisplayName = "Line of Sight Between Actors Consideration", Category = "Considerations"), HideCategories = ("Utility"))
class IAUS_API UIAUSConsideration_LineOfSightBetweenActors : public UIAUSBTDecorator_Consideration
{
	GENERATED_BODY()

public:
	UIAUSConsideration_LineOfSightBetweenActors();

	virtual FString GetStaticDescription() const override;

	virtual float Score(const struct FIAUSBehaviorContext& Context) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Actor A Key"));
	FBlackboardKeySelector ActorAKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Actor B Key"));
	FBlackboardKeySelector ActorBKey;
};
