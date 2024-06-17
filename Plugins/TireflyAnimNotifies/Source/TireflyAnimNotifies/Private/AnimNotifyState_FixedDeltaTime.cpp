// Copyright Tirefly. All Rights Reserved.


#include "AnimNotifyState_FixedDeltaTime.h"


UAnimNotifyState_FixedDeltaTime::UAnimNotifyState_FixedDeltaTime(const FObjectInitializer& ObjectInitializer)
{
	bIsNativeBranchingPoint = true;
}

void UAnimNotifyState_FixedDeltaTime::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);

	ExecutionPoints.Empty();

	if (USkeletalMeshComponent* SkeletalMeshComp = BranchingPointPayload.SkelMeshComponent)
	{
		UWorld* World = SkeletalMeshComp->GetWorld();
		if (FixedTickTimes > 0 && World)
		{
			FixedDeltaTime = 1.0 / 300.0;
			StoredDeltaTime = 0.0;

			const float Start = BranchingPointPayload.NotifyEvent->GetTriggerTime();
			const float Duration = BranchingPointPayload.NotifyEvent->GetDuration();
			const float Sec = Duration / (float)FixedTickTimes;

			ExecutionPoints.Reset();
			for (int32 i = 0; i < FixedTickTimes; i++)
			{
				FFixedTickExecutionPoint ExecutionPoint;
				ExecutionPoint.Frame = Start + Sec * i;
				ExecutionPoint.bExecuted = false;
				ExecutionPoints.Add(ExecutionPoint);
			}
		}
	}
}

void UAnimNotifyState_FixedDeltaTime::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload,
	float FrameDeltaTime)
{
	if (!bEnableTickFixed)
	{
		Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);
		return;
	}

	if (USkeletalMeshComponent* SkeletalMeshComp = BranchingPointPayload.SkelMeshComponent)
	{
		if (UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance())
		{
			if (!ExecutionPoints.IsEmpty())
			{
				FAnimMontageInstance* DestMontageInst = AnimInstance->GetMontageInstanceForID(BranchingPointPayload.MontageInstanceID);
				if (DestMontageInst && DestMontageInst->IsActive() == true)
				{
					int32 AddFrameNumber = 0;
					double TempTime = FrameDeltaTime;
					while (TempTime - FixedDeltaTime > 0.0)
					{
						AddFrameNumber++;
						TempTime -= FixedDeltaTime;
					}
					
					StoredDeltaTime += TempTime;
					if (StoredDeltaTime - FixedDeltaTime > 0.0)
					{
						AddFrameNumber++;
						StoredDeltaTime -= FixedDeltaTime;
					}

					for (int32 i = 0; i < AddFrameNumber; ++i)
					{
						float CurrentPosition = DestMontageInst->GetPosition();
						for (int32 j = 0; j < ExecutionPoints.Num(); ++j)
						{
							if (ExecutionPoints[j].bExecuted == false && ExecutionPoints[j].Frame <= CurrentPosition)
							{
								Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);
								ExecutionPoints[j].bExecuted = true;
							}
						}
					}
				}
			}
		}
	}
}

void UAnimNotifyState_FixedDeltaTime::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (!bEnableTickFixed)
	{
		Super::BranchingPointNotifyEnd(BranchingPointPayload);
		return;
	}

	if (USkeletalMeshComponent* SkeletalMeshComp = BranchingPointPayload.SkelMeshComponent)
	{
		if (SkeletalMeshComp->GetAnimInstance())
		{
			if (!ExecutionPoints.IsEmpty())
			{
				float FrameDeltaTime = SkeletalMeshComp->GetWorld()->GetDeltaSeconds();
				for (FFixedTickExecutionPoint& Point : ExecutionPoints)
				{
					if (Point.bExecuted == false)
					{
						Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);
						Point.bExecuted = true;
					}
				}
			}
		}
	}

	Super::BranchingPointNotifyEnd(BranchingPointPayload);
}
