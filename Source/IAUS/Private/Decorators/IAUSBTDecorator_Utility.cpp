// Copyright Project Borealis

#include "Decorators/IAUSBTDecorator_Utility.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

#include "Composites/IAUSBTComposite_Behavior.h"
#include "Composites/IAUSBTComposite_Utility.h"
#include "Decorators/IAUSBTDecorator_Consideration.h"
#include "IAUSCommon.h"

DECLARE_CYCLE_STAT(TEXT("IAUS Decorator Tick Node"), STAT_IAUSDecoratorTickNode, STATGROUP_IAUS);

UIAUSBTDecorator_Utility::UIAUSBTDecorator_Utility(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/) : Super(ObjectInitializer)
{
	NodeName = TEXT("Utility Evaluator");

	bNotifyTick = true;
	bNotifyActivation = true;
	bNotifyDeactivation = true;

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
		if (UtilityComposite->Children.IsValidIndex(CurrentBehaviorIndex))
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
	}

	EvaluateBehaviors(OwnerComp, MyMemory);
}

void UIAUSBTDecorator_Utility::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                EBTMemoryInit::Type InitType) const
{
	if (InitType == EBTMemoryInit::Initialize)
	{
		FIAUSBTDecorator_UtilityMemory* Memory = CastInstanceNodeMemory<FIAUSBTDecorator_UtilityMemory>(NodeMemory);

		if (const UIAUSBTComposite_Utility* UtilityComposite = Cast<UIAUSBTComposite_Utility>(GetMyNode()))
		{
			// NOTE: const cast is okay because GetNodeMemory effectively does not touch the Node parameter.
			// It's an oversight that the Node parameter is not marged as const. Instead of fixing it in UE, the fix is here.
			Memory->MemoryUtilityComposite = UtilityComposite->CastInstanceNodeMemory<FIAUSBTComposite_UtilityMemory>(
				OwnerComp.GetNodeMemory(const_cast<UIAUSBTComposite_Utility*>(UtilityComposite), OwnerComp.GetActiveInstanceIdx()));
			if (Memory->MemoryUtilityComposite)
			{
				Memory->MemoryUtilityComposite->BehaviorMemories.Empty();

				for (int32 Idx = 0; Idx < UtilityComposite->Children.Num(); Idx++)
				{
					if (const UIAUSBTComposite_Behavior* BehaviorComposite = Cast<UIAUSBTComposite_Behavior>(
						UtilityComposite->Children[Idx].ChildComposite))
					{
						Memory->MemoryUtilityComposite->BehaviorMemories.Add(
							BehaviorComposite->CastInstanceNodeMemory<FIAUSBTComposite_BehaviorMemory>(
								OwnerComp.GetNodeMemory(const_cast<UIAUSBTComposite_Behavior*>(BehaviorComposite), OwnerComp.GetActiveInstanceIdx())));
					}
				}
			}
		}
	}
}

bool UIAUSBTDecorator_Utility::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	FIAUSBTDecorator_UtilityMemory* Memory = CastInstanceNodeMemory<FIAUSBTDecorator_UtilityMemory>(NodeMemory);

	// If this decorator instance isn't active yet it will not be ticked
	// Because of this, we have to evaluate the behaviors now
	if (!Memory->IsRelevant)
	{
		EvaluateBehaviors(OwnerComp, Memory);
	}

	return Memory->IsContextValid;
}

uint16 UIAUSBTDecorator_Utility::GetInstanceMemorySize() const
{
	return sizeof(FIAUSBTDecorator_UtilityMemory);
}

void UIAUSBTDecorator_Utility::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	Super::OnNodeActivation(SearchData);

	FIAUSBTDecorator_UtilityMemory* Memory = GetNodeMemory<FIAUSBTDecorator_UtilityMemory>(SearchData);
	Memory->IsRelevant = true;
}

void UIAUSBTDecorator_Utility::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
	Super::OnNodeDeactivation(SearchData, NodeResult);

	FIAUSBTDecorator_UtilityMemory* Memory = GetNodeMemory<FIAUSBTDecorator_UtilityMemory>(SearchData);
	Memory->IsRelevant = false;
}

void UIAUSBTDecorator_Utility::EvaluateBehaviors(UBehaviorTreeComponent& OwnerComp, FIAUSBTDecorator_UtilityMemory* MyMemory) const
{
	if (!MyMemory || !MyMemory->MemoryUtilityComposite || !OwnerComp.GetAIOwner())
	{
		return;
	}

	FIAUSBTComposite_UtilityMemory* MemoryUtilityComposite = MyMemory->MemoryUtilityComposite;
	const AActor* PreviousActor = MemoryUtilityComposite->Context.Target;

	UpdateBehaviorContext(OwnerComp, MemoryUtilityComposite);

	MyMemory->IsContextValid = MemoryUtilityComposite->Context.TotalScore != 0;
	if (!MyMemory->IsContextValid)
	{
		return;
	}

	const int32 NextBehaviorIndex = MemoryUtilityComposite->Context.BehaviorIndex;
	MemoryUtilityComposite->Evaluator.Behaviors[NextBehaviorIndex].LastExecutionTime = GetWorld()->GetTimeSeconds();

	// If the next behavior is different from the previous or the target is different, stop the execution of the current behavior
	if (NextBehaviorIndex != static_cast<int32>(MemoryUtilityComposite->CurrentChild) || PreviousActor != MemoryUtilityComposite->Context.Target)
	{
		MemoryUtilityComposite->LastBehaviorChangeTime = GetWorld()->GetTimeSeconds();

		OwnerComp.RequestBranchEvaluation(*this);
		//OwnerComp.RequestExecution(EBTNodeResult::Failed);
	}
}

void UIAUSBTDecorator_Utility::UpdateBehaviorContext(UBehaviorTreeComponent& OwnerComp, FIAUSBTComposite_UtilityMemory* MemoryUtilityComposite) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		TArray<AActor*> Targets;
		AIController->GetPerceptionComponent()->GetKnownPerceivedActors(nullptr, Targets);
		Targets.Add(AIController->GetPawn());

		MemoryUtilityComposite->Context = MemoryUtilityComposite->Evaluator.ChooseBehavior(AIController, Targets);
	}
}
