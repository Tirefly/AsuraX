// Copyright tzlFirefly, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect/FireflyGameplayEffect.h"
#include "FireflyGameplayEffect_GenericCooldown.generated.h"


// 通用的能力冷却效果，结合 FireflyGameplayAbility 使用，它的冷却时间和冷却标签都可以在 FireflyGameplayAbility 的派生类中设置
// A universal ability cooldown effect, used in conjunction with FireflyGameplayAbility. Its cooldown duration and tags can be set in derived classes of FireflyGameplayAbility.
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyGameplayEffect_GenericCooldown : public UFireflyGameplayEffect
{
	GENERATED_BODY()

public:
	UFireflyGameplayEffect_GenericCooldown(const FObjectInitializer& ObjectInitializer);

};
