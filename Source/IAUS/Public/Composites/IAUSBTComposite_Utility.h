// Copyright Project Borealis. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTCompositeNode.h"

#include "IAUSEvaluator.h"

#include "IAUSBTComposite_Utility.generated.h"

struct FIAUSBTCompositeUtilityMemory : public FBTCompositeMemory
{
	IAUSEvaluator Evaluator;
	FIAUSBehaviorContext Context;
	float LastBehaviorChangeTime;
	TArray<struct FIAUSBTCompositeBehaviorMemory*> BehaviorMemories;
};

UCLASS()
class IAUS_API UIAUSBTComposite_Utility : public UBTCompositeNode
{
	GENERATED_BODY()

public:
	UIAUSBTComposite_Utility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

protected:
	/* Primary Target Enemy */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector BlackboardTargetKey;

	/* Last Known Position of Target */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector BlackboardLKPKey;

	virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;

	virtual uint16 GetInstanceMemorySize() const override;

	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
};
