// Copyright Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"

#include "IAUSBTDecorator_Utility.generated.h"

struct FIAUSBTComposite_UtilityMemory;

struct FIAUSBTDecorator_UtilityMemory
{
	bool IsContextInvalid;
	FIAUSBTComposite_UtilityMemory* MemoryUtilityComposite;
};

UCLASS()
class IAUS_API UIAUSBTDecorator_Utility : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UIAUSBTDecorator_Utility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** Initializes the memories of all the IAUS nodes */
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual uint16 GetInstanceMemorySize() const override;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void EvaluateBehaviors(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

	/** Finds the targets, lets the evaluator choose a new behavior, and updates the Context inside the MemoryUtilityComposite */
	virtual void UpdateBehaviorContext(UBehaviorTreeComponent& OwnerComp, FIAUSBTComposite_UtilityMemory* MemoryUtilityComposite) const;

	/** How often a new behavior can be selected */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	float SelectionCooldown;
};
