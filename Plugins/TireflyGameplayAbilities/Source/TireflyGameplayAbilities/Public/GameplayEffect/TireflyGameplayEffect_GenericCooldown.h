// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "TireflyGameplayEffect_GenericCooldown.generated.h"


// 通用的能力冷却效果，结合 TireflyGameplayAbility 使用，它的冷却时间和冷却标签都可以在 TireflyGameplayAbility 的派生类中设置
UCLASS(DisplayName = "Generic Cooldown Effect")
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayEffect_GenericCooldown : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UTireflyGameplayEffect_GenericCooldown(const FObjectInitializer& ObjectInitializer);
};
