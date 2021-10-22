// Copyright Project Borealis. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTCompositeNode.h"

#include "IAUSEvaluator.h"

#include "IAUSBTComposite_Utility.generated.h"

struct FIAUSBTComposite_UtilityMemory : public FBTCompositeMemory
{
	IAUSEvaluator Evaluator;
	FIAUSBehaviorContext Context;
	float LastBehaviorChangeTime;
	TArray<struct FIAUSBTComposite_BehaviorMemory*> BehaviorMemories;
};

UCLASS()
class IAUS_API UIAUSBTComposite_Utility : public UBTCompositeNode
{
	GENERATED_BODY()

public:
	UIAUSBTComposite_Utility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual uint16 GetInstanceMemorySize() const override;

protected:
	virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;

	/* Blackboard key where the target is stored */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector BlackboardTargetKey;

	/* Blackboard key where the last known position of the target is stored */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector BlackboardLKPKey;
};
