// Copyright Project Borealis. All rights reserved.

#include "IAUS/Public/Composites/IAUSBTComposite_Utility.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "IAUS/Public/Composites/IAUSBTComposite_Behavior.h"
#include "IAUS/Public/Decorators/IAUSBTDecorator_Consideration.h"

UIAUSBTComposite_Utility::UIAUSBTComposite_Utility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Utility";
}

void UIAUSBTComposite_Utility::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	if (InitType == EBTMemoryInit::Initialize)
	{
		FIAUSBTCompositeUtilityMemory* Memory = reinterpret_cast<FIAUSBTCompositeUtilityMemory*>(NodeMemory);
		Memory->OwnerComp = &OwnerComp;

		for (int32 Idx = 0; Idx < GetChildrenNum(); ++Idx)
		{
			const UIAUSBTComposite_Behavior* BehaviorComposite = Cast<UIAUSBTComposite_Behavior>(Children[Idx].ChildComposite);
			if (!BehaviorComposite)
			{
				continue;
			}

			FIAUSBehavior Behavior;
			Behavior.bTargetSelf = BehaviorComposite->bTargetSelf;
			Behavior.bTargetFriendly = BehaviorComposite->bTargetFriendly;
			Behavior.bTargetNeutral = BehaviorComposite->bTargetNeutral;
			Behavior.bTargetHostile = BehaviorComposite->bTargetHostile;
			Behavior.InitialWeight = BehaviorComposite->InitialWeight;

			for (auto Decorator : Children[Idx].Decorators)
			{
				UIAUSBTDecorator_Consideration* Consideration = Cast<UIAUSBTDecorator_Consideration>(Decorator);
				if (Consideration)
				{
					Behavior.Considerations.Add(Consideration);
				}
			}

			Memory->Evaluator.AddBehavior(Behavior, BehaviorComposite->GetNodeName());
		}
	}
}

void UIAUSBTComposite_Utility::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBActorAsset = GetBlackboardAsset();
	if (ensure(BBActorAsset))
	{
		BlackboardTargetKey.ResolveSelectedKey(*BBActorAsset);
		BlackboardLKPKey.ResolveSelectedKey(*BBActorAsset);
	}
}

int32 UIAUSBTComposite_Utility::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	FIAUSBTCompositeUtilityMemory* Memory = GetNodeMemory<FIAUSBTCompositeUtilityMemory>(SearchData);

	// success = quit
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;

	AActor* TargetActor = nullptr;

	// If the last behavior failed and is the same as the next, return to parent to prevent an infinite loop
	if (!Memory->LastBehaviorFailed || PrevChild != Memory->Context.BehaviorIndex || LastResult != EBTNodeResult::Failed)
	{
		if (Memory->Context.TotalScore != 0)
		{
			NextChildIdx = Memory->Context.BehaviorIndex;
			TargetActor = Memory->Context.Actor;
		}
	}

	Memory->LastBehaviorFailed = (LastResult == EBTNodeResult::Failed);

	SearchData.OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardTargetKey.GetSelectedKeyID(), TargetActor);
	if (TargetActor)
	{
		SearchData.OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardLKPKey.SelectedKeyName, TargetActor->GetActorLocation());
	}

	return NextChildIdx;
}

uint16 UIAUSBTComposite_Utility::GetInstanceMemorySize() const
{
	return sizeof(FIAUSBTCompositeUtilityMemory);
}
