// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_FixedDeltaTime.generated.h"


USTRUCT()
struct FFixedTickExecutionPoint
{
	GENERATED_BODY()

public:
	float Frame = -1.f;
	bool bExecuted = false;	
};


// 能够按照设定好的Tick时间，在持续时间内均匀的执行逻辑的AnimNotifyState
UCLASS(Abstract)
class TIREFLYANIMNOTIFIES_API UAnimNotifyState_FixedDeltaTime : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_FixedDeltaTime(const FObjectInitializer& ObjectInitializer);

	virtual void BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual void BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime) override;
	virtual void BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload) override;

protected:
	// 是否启用按照指定频率均匀执行逻辑
	UPROPERTY(EditAnywhere, Category = "Fixed Delta Time")
	bool bEnableTickFixed = true;
	
	// 均匀执行的Tick时间
	UPROPERTY(EditAnywhere, Category = "Fixed Delta Time", Meta = (EditCondition = "bEnableTickFixed", EditConditionHides, UIMin =  "1", ClampMin = "1"))
	int32 FixedTickTimes = 1.f;

private:
	// 是否启用按照指定Tick时间，均匀执行逻辑
	double FixedDeltaTime = 0.f;

	// 多余的时间
	double StoredDeltaTime = 0.f;
	
	UPROPERTY()
	TArray<FFixedTickExecutionPoint> ExecutionPoints;
};
