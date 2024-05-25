// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "TireflyAbilityParam_MontagePlay.generated.h"


// GameplayAbility的蒙太奇播放参数
UCLASS(DisplayName = "Montage Play")
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_MontagePlay
	: public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// 蒙太奇播放参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> MontageToPlay = nullptr;

	// 蒙太奇播放速率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayRate = 1.f;

	// 蒙太奇开始播放的片段
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StartSection = NAME_None;

	// 蒙太奇播放结束的时候是否停止
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bStopWhenAbilityEnds = true;

	// 蒙太奇的根运动缩放
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimRootMotionTranslationScale = 1.f;

	// 蒙太奇开始播放的时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartTimeSeconds = 0.f;

public:
	virtual bool IsShowcaseTextEditable_Implementation() const override { return false; }
};
