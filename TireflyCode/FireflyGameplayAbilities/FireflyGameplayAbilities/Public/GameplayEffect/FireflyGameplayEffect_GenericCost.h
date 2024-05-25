// Copyright tzlFirefly, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect/FireflyGameplayEffect.h"
#include "FireflyGameplayEffect_GenericCost.generated.h"


// 通用的能力消耗效果，结合 FireflyGameplayAbility 使用，它的属性资源消耗都可以在 FireflyGameplayAbility 的派生类中设置
// A universal ability cost effect used in conjunction with FireflyGameplayAbility. Its attribute resource costs can be set in derived classes of FireflyGameplayAbility.
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyGameplayEffect_GenericCost : public UFireflyGameplayEffect
{
	GENERATED_BODY()

public:
	UFireflyGameplayEffect_GenericCost(const FObjectInitializer& ObjectInitializer);
	
};
