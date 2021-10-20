// Copyright Project Borealis. All rights reserved.

#include "Decorators/IAUSBTDecorator_Utility.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

#include "Composites/IAUSBTComposite_Behavior.h"
#include "Composites/IAUSBTComposite_Utility.h"
#include "Decorators/IAUSBTDecorator_Consideration.h"
#include "IAUS/IAUSCommon.h"

DECLARE_CYCLE_STAT(TEXT("IAUS Decorator Tick Node"), STAT_IAUSDecoratorTickNode, STATGROUP_IAUS);

UIAUSBTDecorator_Utility::UIAUSBTDecorator_Utility(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/) : Super(ObjectInitializer)
{
	NodeName = TEXT("Utility Ticker");

	bNotifyTick = true;
	bNotifyActivation = true;

	SelectionCooldown = 0.25f;
}

void UIAUSBTDecorator_Utility::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	SCOPE_CYCLE_COUNTER(STAT_IAUSDecoratorTickNode);

	FIAUSBTUtilityDecoratorMemory* UtilityDecoratorMemory = CastInstanceNodeMemory<FIAUSBTUtilityDecoratorMemory>(NodeMemory);
	if (!UtilityDecoratorMemory || !UtilityDecoratorMemory->ParentMemory)
	{
		return;
	}

	FIAUSBTCompositeUtilityMemory* UtilityCompositeMemory = UtilityDecoratorMemory->ParentMemory;

	// Give behavior a moment to stabilize before we start looking for other things to do
	if (UtilityCompositeMemory->LastBehaviorChangeTime + SelectionCooldown >= GetWorld()->GetTimeSeconds())
	{
		return;
	}

	const int32 CurrentBehaviorIndex = UtilityCompositeMemory->Context.BehaviorIndex;

	if (const UIAUSBTComposite_Utility* UtilityComposite = Cast<UIAUSBTComposite_Utility>(GetMyNode()))
	{
		if (const UIAUSBTComposite_Behavior* CurrentBehaviorComposite = Cast<UIAUSBTComposite_Behavior>(UtilityComposite->Children[CurrentBehaviorIndex].ChildComposite))
		{
			if (!CurrentBehaviorComposite->bInterruptible)
			{
				if (UtilityCompositeMemory->BehaviorMemories[CurrentBehaviorIndex]->bExecuting)
				{
					return;
				}
			}
		}
	}

	SelectBehavior(OwnerComp, NodeMemory);
}

void UIAUSBTDecorator_Utility::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	const UIAUSBTComposite_Utility* UtilityComposite = Cast<UIAUSBTComposite_Utility>(GetMyNode());
	if (!UtilityComposite)
	{
		return;
	}

	if (FIAUSBTUtilityDecoratorMemory* MyMemory = GetNodeMemory<FIAUSBTUtilityDecoratorMemory>(SearchData))
	{
		if (FIAUSBTCompositeUtilityMemory* UtilityCompositeMemory = MyMemory->ParentMemory = UtilityComposite->GetNodeMemory<FIAUSBTCompositeUtilityMemory>(SearchData))
		{
			UtilityCompositeMemory->BehaviorMemories.Empty();

			for (int32 Idx = 0; Idx < UtilityComposite->Children.Num(); Idx++)
			{
				if (const UIAUSBTComposite_Behavior* BehaviorComposite = Cast<UIAUSBTComposite_Behavior>(UtilityComposite->Children[Idx].ChildComposite))
				{
					UtilityCompositeMemory->BehaviorMemories.Add(BehaviorComposite->GetNodeMemory<FIAUSBTCompositeBehaviorMemory>(SearchData));
				}
			}
		}
	}
}

bool UIAUSBTDecorator_Utility::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	FIAUSBTUtilityDecoratorMemory* DecoratorMemory = CastInstanceNodeMemory<FIAUSBTUtilityDecoratorMemory>(NodeMemory);

	return !DecoratorMemory->IsInvalid;
}

uint16 UIAUSBTDecorator_Utility::GetInstanceMemorySize() const
{
	return sizeof(FIAUSBTUtilityDecoratorMemory);
}

void UIAUSBTDecorator_Utility::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	SelectBehavior(OwnerComp, NodeMemory);
}

void UIAUSBTDecorator_Utility::SelectBehavior(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	FIAUSBTUtilityDecoratorMemory* UtilityDecoratorMemory = CastInstanceNodeMemory<FIAUSBTUtilityDecoratorMemory>(NodeMemory);
	if (!UtilityDecoratorMemory || !UtilityDecoratorMemory->ParentMemory || !OwnerComp.GetAIOwner())
	{
		return;
	}

	FIAUSBTCompositeUtilityMemory* UtilityCompositeMemory = UtilityDecoratorMemory->ParentMemory;
	int32 CurrentBehaviorIndex = UtilityCompositeMemory->Context.BehaviorIndex;
	const AActor* CurrentActor = UtilityCompositeMemory->Context.Actor;

	UpdateBehaviorContext(OwnerComp, UtilityCompositeMemory);

	if (UtilityCompositeMemory->Context.TotalScore == 0)
	{
		UtilityDecoratorMemory->IsInvalid = true;
		return;
	}

	UtilityDecoratorMemory->IsInvalid = false;

	CurrentBehaviorIndex = UtilityCompositeMemory->Context.BehaviorIndex;
	UtilityCompositeMemory->Evaluator.Behaviors[CurrentBehaviorIndex].LastExecutionTime = GetWorld()->GetTimeSeconds();

	if (CurrentBehaviorIndex != static_cast<int32>(UtilityCompositeMemory->CurrentChild) || CurrentActor != UtilityCompositeMemory->Context.Actor)
	{
		UtilityCompositeMemory->LastBehaviorChangeTime = GetWorld()->GetTimeSeconds();

		OwnerComp.RequestExecution(EBTNodeResult::Failed);
	}
}

void UIAUSBTDecorator_Utility::UpdateBehaviorContext(UBehaviorTreeComponent& OwnerComp, FIAUSBTCompositeUtilityMemory* UtilityCompositeMemory) const
{
	if (AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner()))
	{
		TArray<AActor*> Targets;
		AIController->GetPerceptionComponent()->GetKnownPerceivedActors(nullptr, Targets);
		Targets.Add(AIController->GetPawn());

		UtilityCompositeMemory->Context = UtilityCompositeMemory->Evaluator.ChooseBehavior(AIController, Targets);
	}
}
