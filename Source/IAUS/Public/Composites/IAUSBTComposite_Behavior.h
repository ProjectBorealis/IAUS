// Copyright Project Borealis. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Composites/BTComposite_Selector.h"

#include "IAUSBTComposite_Behavior.generated.h"

struct FIAUSBTComposite_BehaviorMemory : public FBTCompositeMemory
{
	bool bExecuting = false;
};

UCLASS()
class IAUS_API UIAUSBTComposite_Behavior : public UBTComposite_Selector
{
	GENERATED_BODY()

public:
	UIAUSBTComposite_Behavior(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, Category = "Utility")
	bool bTargetSelf = false;

	UPROPERTY(EditAnywhere, Category = "Utility")
	bool bTargetFriendly = false;

	UPROPERTY(EditAnywhere, Category = "Utility")
	bool bTargetNeutral = false;

	UPROPERTY(EditAnywhere, Category = "Utility")
	bool bTargetHostile = true;

	UPROPERTY(EditAnywhere, Category = "Utility")
	bool bInterruptible = true;

	UPROPERTY(EditAnywhere, Category = "Utility", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float InitialWeight = 1.0;

#if WITH_EDITOR
	FString GetStaticDescription() const override;
#endif

protected:
	virtual void NotifyNodeActivation(FBehaviorTreeSearchData& SearchData) const override;

	virtual void NotifyNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult) const override;

	virtual uint16 GetInstanceMemorySize() const override;
};
