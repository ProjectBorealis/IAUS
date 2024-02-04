// Copyright Project Borealis

#include "Composites/IAUSBTComposite_Utility.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Composites/IAUSBTComposite_Behavior.h"
#include "Decorators/IAUSBTDecorator_Consideration.h"

UIAUSBTComposite_Utility::UIAUSBTComposite_Utility(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/) : Super(ObjectInitializer)
{
	NodeName = TEXT("Utility Selector");
	bUseChildExecutionNotify = true;
}

void UIAUSBTComposite_Utility::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	if (InitType == EBTMemoryInit::Initialize)
	{
		FIAUSBTComposite_UtilityMemory* Memory = CastInstanceNodeMemory<FIAUSBTComposite_UtilityMemory>(NodeMemory);
		Memory->Context = {};
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
			Behavior.CompensationFactor = 1.0f - (1.0f / Children[Idx].Decorators.Num());

			for (const auto& Decorator : Children[Idx].Decorators)
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

uint16 UIAUSBTComposite_Utility::GetInstanceMemorySize() const
{
	return sizeof(FIAUSBTComposite_UtilityMemory);
}

int32 UIAUSBTComposite_Utility::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	FIAUSBTComposite_UtilityMemory* Memory = GetNodeMemory<FIAUSBTComposite_UtilityMemory>(SearchData);
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;
	AActor* Target = nullptr;
	const int32 CurrentBehaviorIndex = Memory->Context.BehaviorIndex;
	const UIAUSBTComposite_Behavior* CurrentBehavior = Children.IsValidIndex(CurrentBehaviorIndex)
		                                                   ? Cast<UIAUSBTComposite_Behavior>(
			                                                   Children[CurrentBehaviorIndex].ChildComposite)
		                                                   : nullptr;

	// Reasons why the current behavior may not be valid for execution:
	// - It's the same as the previous and it's non-interruptible.
	// - It's the same as the previous behavior and it already failed to execute.
	// - Its score is not above 0.
	const bool bSameAsPrevious = PrevChild == CurrentBehaviorIndex;
	if (!(bSameAsPrevious && CurrentBehavior && !CurrentBehavior->bInterruptible) &&
		!(bSameAsPrevious && LastResult == EBTNodeResult::Failed) && (Memory->Context.TotalScore > 0.f))
	{
		NextChildIdx = CurrentBehaviorIndex;
		Target = Memory->Context.Target;
	}

	SearchData.OwnerComp.GetBlackboardComponent()->SetValueAsObject(BlackboardTargetKey.SelectedKeyName, Target);
	if (Target)
	{
		SearchData.OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardLKPKey.SelectedKeyName, Target->GetActorLocation());
	}

	return NextChildIdx;
}

void UIAUSBTComposite_Utility::NotifyChildExecution(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                    int32 ChildIdx, EBTNodeResult::Type& NodeResult) const
{
	Super::NotifyChildExecution(OwnerComp, NodeMemory, ChildIdx, NodeResult);

	FIAUSBTComposite_UtilityMemory* Memory = CastInstanceNodeMemory<FIAUSBTComposite_UtilityMemory>(NodeMemory);
	if (Memory->Evaluator.Behaviors.IsValidIndex(Memory->Context.BehaviorIndex))
	{
		Memory->Evaluator.Behaviors[Memory->Context.BehaviorIndex].LastExecutionTime = GetWorld()->GetTimeSeconds();
	}
}
