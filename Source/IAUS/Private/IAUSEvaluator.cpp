// Copyright Project Borealis

#include "IAUSEvaluator.h"

#include "AIController.h"

#include "Decorators/IAUSBTDecorator_Consideration.h"
#include "IAUSCommon.h"

DECLARE_CYCLE_STAT(TEXT("IAUS Choose Behavior"), STAT_IAUSChooseBehavior, STATGROUP_IAUS);

void IAUSEvaluator::AddBehavior(const FIAUSBehavior& Behavior, const FString& Name)
{
	int32 Index = Behaviors.Add(Behavior);
	BehaviorNameIndexMap.Add(Name, Index);
}

FIAUSBehaviorContext IAUSEvaluator::ChooseBehavior(AAIController* AIController, const TArray<AActor*> Targets, bool bCheckTeamAttitude /*= true*/)
{
	SCOPE_CYCLE_COUNTER(STAT_IAUSChooseBehavior);

	FIAUSBehaviorContext BestContext = {};
	BestContext.TotalScore = 0;

	for (int32 Idx = 0; Idx < Behaviors.Num(); Idx++)
	{
		for (AActor* Target : Targets)
		{
			if (AIController->GetPawn() == Target)
			{
				if (!Behaviors[Idx].bTargetSelf)
				{
					continue;
				}
			}
			else if (bCheckTeamAttitude)
			{
				ETeamAttitude::Type Attitude = AIController->GetTeamAttitudeTowards(*Target);
				if ((Attitude == ETeamAttitude::Friendly && !Behaviors[Idx].bTargetFriendly) ||
					(Attitude == ETeamAttitude::Neutral && !Behaviors[Idx].bTargetNeutral) || (Attitude == ETeamAttitude::Hostile && !Behaviors[Idx].bTargetHostile))
				{
					continue;
				}
			}

			FIAUSBehaviorContext Context;
			Context.Evaluator = this;
			Context.BehaviorIndex = Idx;
			Context.AIController = AIController;
			Context.Target = Target;
			Context.TotalScore = Behaviors[Idx].InitialWeight;

			for (auto Consideration : Behaviors[Idx].Considerations)
			{
				const float Score = Consideration->Score(Context);
				if (Score <= 0)
				{
					Context.TotalScore = 0;
					break;
				}

				const float Modification = (1.0 - Score) * Behaviors[Idx].CompensationFactor;
				Context.TotalScore *= Score + (Modification * Score);
			}

			if (Context.TotalScore == 1)
			{
				return Context;
			}

			if (Context.TotalScore > BestContext.TotalScore)
			{
				BestContext = Context;
			}
		}
	}

	return BestContext;
}
