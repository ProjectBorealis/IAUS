// Copyright Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTDecorator.h"

#include "IAUSResponseCurve.h"

#include "IAUSBTDecorator_Consideration.generated.h"

UCLASS(HideCategories = (Condition))
class IAUS_API UIAUSBTDecorator_Consideration : public UBTDecorator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Utility")
	TObjectPtr<UIAUSResponseCurve> ResponseCurve;

	virtual float Score(const struct FIAUSBehaviorContext& Context) const;
};
