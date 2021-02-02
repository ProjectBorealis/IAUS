// Copyright 2017-2020 Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"

#include "IAUSBTDecorator_Utility.generated.h"

struct FIAUSBTCompositeUtilityMemory;

struct FIAUSBTUtilityDecoratorMemory
{
	bool IsInvalid;
	FIAUSBTCompositeUtilityMemory* ParentMemory;
};

/**
 *
 */
UCLASS()
class IAUS_API UIAUSBTDecorator_Utility : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual uint16 GetInstanceMemorySize() const override;

	/** Inertia weight. This is the time we allow a behavior
		to stabilize before we attempt to choose a new behavior */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	float IntertiaWeight;

public:
	UIAUSBTDecorator_Utility(const FObjectInitializer& ObjectInitializer);
};
