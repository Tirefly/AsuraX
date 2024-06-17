// Copyright Tirefly. All Rights Reserved.


#include "AnimNotifyState_FixedExecutionNumber.h"


UAnimNotifyState_FixedExecutionNumber::UAnimNotifyState_FixedExecutionNumber(const FObjectInitializer& ObjectInitializer)
{
	bIsNativeBranchingPoint = true;
}

void UAnimNotifyState_FixedExecutionNumber::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);

	ExecutionPoints.Empty();
	if (!bEnableExecutionFixed)
	{
		return;
	}

	if (USkeletalMeshComponent* SkeletalMeshComp = BranchingPointPayload.SkelMeshComponent)
	{
		if (SkeletalMeshComp->GetAnimInstance())
		{
			float StartTime = BranchingPointPayload.NotifyEvent->GetTriggerTime();
			float EndTime = BranchingPointPayload.NotifyEvent->GetEndTriggerTime();

			if (UAnimSequenceBase* Animation = BranchingPointPayload.SequenceAsset)
			{
				int32 StartFrame = GetFrameAtTime(Animation, StartTime);
				int32 EndFrame = GetFrameAtTime(Animation, EndTime);

				while (StartFrame <= EndFrame)
				{
					FFixedNumberExecutionPoint& ExecutionPoint = ExecutionPoints.AddDefaulted_GetRef();
					ExecutionPoint.Frame = StartFrame;
					ExecutionPoint.AnimStateNormalizedTime = GetTimeAtFrame(Animation, StartFrame);
					ExecutionPoint.bExecuted = false;

					StartFrame += (EndFrame / ExecutionFrequency);
				}
			}
		}
	}
}

void UAnimNotifyState_FixedExecutionNumber::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload,
	float FrameDeltaTime)
{
	if (!bEnableExecutionFixed)
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
				FAnimMontageInstance* MontageInstance = AnimInstance->GetMontageInstanceForID(BranchingPointPayload.MontageInstanceID);
				if (MontageInstance->IsValid() && MontageInstance->IsPlaying())
				{
					for (FFixedNumberExecutionPoint& Point : ExecutionPoints)
					{
						if ((Point.bExecuted == false) && (Point.AnimStateNormalizedTime <= MontageInstance->GetPosition()))
						{
							Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);
							Point.bExecuted = true;
						}
					}
				}
			}
		}
	}
}

void UAnimNotifyState_FixedExecutionNumber::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (!bEnableExecutionFixed)
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
				for (FFixedNumberExecutionPoint& Point : ExecutionPoints)
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

int32 UAnimNotifyState_FixedExecutionNumber::GetFrameAtTime(UAnimSequenceBase* Animation, const float Time) const
{
	return FMath::Clamp<int32>(Animation->GetSamplingFrameRate().AsFrameTime(Time).RoundToFrame().Value,
		0, Animation->GetNumberOfSampledKeys() - 1);
}

float UAnimNotifyState_FixedExecutionNumber::GetTimeAtFrame(UAnimSequenceBase* Animation, const int32 Frame) const
{
	return FMath::Clamp<float>(Animation->GetSamplingFrameRate().AsSeconds(Frame), 0.f, Animation->GetPlayLength());
}
