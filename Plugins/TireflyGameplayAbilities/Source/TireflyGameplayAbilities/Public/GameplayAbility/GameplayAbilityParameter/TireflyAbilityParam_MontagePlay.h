// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "TireflyAbilityParam_MontagePlay.generated.h"


// GameplayAbility的蒙太奇播放参数
UCLASS(DisplayName = "Montage Play")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_MontagePlay
	: public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// 蒙太奇播放参数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> MontageToPlay = nullptr;

	// 蒙太奇播放速率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PlayRate = 1.f;

	// 蒙太奇开始播放的片段
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName StartSection = NAME_None;

	// 蒙太奇播放结束的时候是否停止
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bStopWhenAbilityEnds = true;

	// 蒙太奇的根运动缩放
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AnimRootMotionTranslationScale = 1.f;

	// 蒙太奇开始播放的时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StartTimeSeconds = 0.f;

public:
	virtual bool IsShowcaseTextEditable_Implementation() const override { return false; }
};
