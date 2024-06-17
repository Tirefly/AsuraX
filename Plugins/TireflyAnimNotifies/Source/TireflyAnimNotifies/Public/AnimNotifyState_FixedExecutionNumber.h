// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_FixedExecutionNumber.generated.h"


USTRUCT()
struct FFixedNumberExecutionPoint
{
	GENERATED_BODY()

public:
	float Frame = -1.f;
	float AnimStateNormalizedTime = 0.f;
	bool bExecuted = false;	
};


// 能够按照设定好的频率，在持续时间内均匀的执行逻辑的AnimNotifyState
UCLASS(Abstract)
class TIREFLYANIMNOTIFIES_API UAnimNotifyState_FixedExecutionNumber : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_FixedExecutionNumber(const FObjectInitializer& ObjectInitializer);

	virtual void BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual void BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime) override;
	virtual void BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload) override;

	/** Get the frame number for the provided time */
	virtual int32 GetFrameAtTime(UAnimSequenceBase* Animation, const float Time) const;

	/** Get the time at the given frame */
	virtual float GetTimeAtFrame(UAnimSequenceBase* Animation, const int32 Frame) const;

protected:
	// 是否启用按照指定频率均匀执行逻辑
	UPROPERTY(EditAnywhere, Category = "Fixed Execution Number")
	bool bEnableExecutionFixed = true;

	// 均匀执行的频率
	UPROPERTY(EditAnywhere, Category = "Fixed Execution Number", Meta = (EditCondition = "bEnableExecutionFixed", EditConditionHides, UIMin =  "1", ClampMin = "1"))
	int32 ExecutionFrequency = 1;

private:
	UPROPERTY()
	TArray<FFixedNumberExecutionPoint> ExecutionPoints;
};
