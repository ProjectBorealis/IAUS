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
	NodeName = TEXT("Utility Evaluator");

	bNotifyTick = true;
	bNotifyActivation = true;

	SelectionCooldown = 0.25f;
}

void UIAUSBTDecorator_Utility::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	SCOPE_CYCLE_COUNTER(STAT_IAUSDecoratorTickNode);

	FIAUSBTDecorator_UtilityMemory* MyMemory = CastInstanceNodeMemory<FIAUSBTDecorator_UtilityMemory>(NodeMemory);
	if (!MyMemory || !MyMemory->MemoryUtilityComposite)
	{
		return;
	}

	FIAUSBTComposite_UtilityMemory* MemoryUtilityComposite = MyMemory->MemoryUtilityComposite;

	// Give the current behavior a moment to stabilize before we start looking for a new behavior
	if (MemoryUtilityComposite->LastBehaviorChangeTime + SelectionCooldown >= GetWorld()->GetTimeSeconds())
	{
		return;
	}

	const int32 CurrentBehaviorIndex = MemoryUtilityComposite->Context.BehaviorIndex;

	if (const UIAUSBTComposite_Utility* UtilityComposite = Cast<UIAUSBTComposite_Utility>(GetMyNode()))
	{
		if (const UIAUSBTComposite_Behavior* CurrentBehaviorComposite = Cast<UIAUSBTComposite_Behavior>(UtilityComposite->Children[CurrentBehaviorIndex].ChildComposite))
		{
			// If the current behavior is uninterruptible and it's executing, don't look for a new behavior
			if (!CurrentBehaviorComposite->bInterruptible && MemoryUtilityComposite->BehaviorMemories[CurrentBehaviorIndex]->bExecuting)
			{
				return;
			}
		}
	}

	EvaluateBehaviors(OwnerComp, NodeMemory);
}

void UIAUSBTDecorator_Utility::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	if (FIAUSBTDecorator_UtilityMemory* MyMemory = GetNodeMemory<FIAUSBTDecorator_UtilityMemory>(SearchData))
	{
		if (const UIAUSBTComposite_Utility* UtilityComposite = Cast<UIAUSBTComposite_Utility>(GetMyNode()))
		{
			MyMemory->MemoryUtilityComposite = UtilityComposite->GetNodeMemory<FIAUSBTComposite_UtilityMemory>(SearchData);
			if (FIAUSBTComposite_UtilityMemory* MemoryUtilityComposite = MyMemory->MemoryUtilityComposite)
			{
				MemoryUtilityComposite->BehaviorMemories.Empty();

				for (int32 Idx = 0; Idx < UtilityComposite->Children.Num(); Idx++)
				{
					if (const UIAUSBTComposite_Behavior* BehaviorComposite = Cast<UIAUSBTComposite_Behavior>(UtilityComposite->Children[Idx].ChildComposite))
					{
						MemoryUtilityComposite->BehaviorMemories.Add(BehaviorComposite->GetNodeMemory<FIAUSBTComposite_BehaviorMemory>(SearchData));
					}
				}
			}
		}
	}
}

bool UIAUSBTDecorator_Utility::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	FIAUSBTDecorator_UtilityMemory* Memory = CastInstanceNodeMemory<FIAUSBTDecorator_UtilityMemory>(NodeMemory);

	return !Memory->IsContextInvalid;
}

uint16 UIAUSBTDecorator_Utility::GetInstanceMemorySize() const
{
	return sizeof(FIAUSBTDecorator_UtilityMemory);
}

void UIAUSBTDecorator_Utility::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	EvaluateBehaviors(OwnerComp, NodeMemory);
}

void UIAUSBTDecorator_Utility::EvaluateBehaviors(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	FIAUSBTDecorator_UtilityMemory* MyMemory = CastInstanceNodeMemory<FIAUSBTDecorator_UtilityMemory>(NodeMemory);
	if (!MyMemory || !MyMemory->MemoryUtilityComposite || !OwnerComp.GetAIOwner())
	{
		return;
	}

	FIAUSBTComposite_UtilityMemory* MemoryUtilityComposite = MyMemory->MemoryUtilityComposite;
	const AActor* PreviousActor = MemoryUtilityComposite->Context.Target;

	UpdateBehaviorContext(OwnerComp, MemoryUtilityComposite);

	if (MemoryUtilityComposite->Context.TotalScore == 0)
	{
		MyMemory->IsContextInvalid = true;
		return;
	}

	MyMemory->IsContextInvalid = false;

	const int32 NextBehaviorIndex = MemoryUtilityComposite->Context.BehaviorIndex;
	MemoryUtilityComposite->Evaluator.Behaviors[NextBehaviorIndex].LastExecutionTime = GetWorld()->GetTimeSeconds();

	// If the next behavior is different from the previous or the target is different, stop the execution of the current behavior
	if (NextBehaviorIndex != static_cast<int32>(MemoryUtilityComposite->CurrentChild) || PreviousActor != MemoryUtilityComposite->Context.Target)
	{
		MemoryUtilityComposite->LastBehaviorChangeTime = GetWorld()->GetTimeSeconds();

		OwnerComp.RequestExecution(EBTNodeResult::Failed);
	}
}

void UIAUSBTDecorator_Utility::UpdateBehaviorContext(UBehaviorTreeComponent& OwnerComp, FIAUSBTComposite_UtilityMemory* MemoryUtilityComposite) const
{
	if (AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner()))
	{
		TArray<AActor*> Targets;
		AIController->GetPerceptionComponent()->GetKnownPerceivedActors(nullptr, Targets);
		Targets.Add(AIController->GetPawn());

		MemoryUtilityComposite->Context = MemoryUtilityComposite->Evaluator.ChooseBehavior(AIController, Targets);
	}
}
