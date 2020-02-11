// Copyright 2017-2020 Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "IAUSEvaluator.h"

#include "BehaviorTree/BTCompositeNode.h"

#include "IAUSBTComposite_Utility.generated.h"

struct FIAUSBTCompositeUtilityMemory : public FBTCompositeMemory
{
	IAUSEvaluator Evaluator;
	FIAUSBehaviorContext Context;
	float LastBehaviorChangeTime;

	UBehaviorTreeComponent* OwnerComp;
};

/**
 *
 */
UCLASS()
class IAUS_API UIAUSBTComposite_Utility : public UBTCompositeNode
{
	GENERATED_UCLASS_BODY()

	/** initialize any asset related data */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	FName GetSelectedBlackboardKey() const;

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

FORCEINLINE FName UIAUSBTComposite_Utility::GetSelectedBlackboardKey() const
{
	return BlackboardTargetKey.SelectedKeyName;
}
