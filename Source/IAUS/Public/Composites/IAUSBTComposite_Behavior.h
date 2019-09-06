// Copyright 2017-2019 Project Borealis

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Composites/BTComposite_Selector.h"
#include "IAUSBTComposite_Behavior.generated.h"

/**
 * 
 */
UCLASS()
class IAUS_API UIAUSBTComposite_Behavior : public UBTComposite_Selector
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Utility")
	bool bTargetSelf = false;

	UPROPERTY(EditAnywhere, Category = "Utility")
	bool bTargetFriendly = false;

	UPROPERTY(EditAnywhere, Category = "Utility")
	bool bTargetNeutral = false;

	UPROPERTY(EditAnywhere, Category = "Utility")
	bool bTargetHostile = true;

	UPROPERTY(EditAnywhere, Category = "Utility", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float InitialWeight = 1.0;

#if WITH_EDITOR
	FString GetStaticDescription() const override
	{
		FString TargetDesc;

		if (bTargetSelf)
		{
			TargetDesc.Append("Target Self\n");
		}
		if (bTargetFriendly)
		{
			TargetDesc.Append("Target Friendly\n");
		}
		if (bTargetNeutral)
		{
			TargetDesc.Append("Target Neutral\n");
		}
		if (bTargetHostile)
		{
			TargetDesc.Append("Target Hostile\n");
		}

		return FString::Printf(TEXT("%s\n%s\nInitial Weight: %f"), *Super::GetStaticDescription(), *TargetDesc, InitialWeight);
	}
#endif
};
