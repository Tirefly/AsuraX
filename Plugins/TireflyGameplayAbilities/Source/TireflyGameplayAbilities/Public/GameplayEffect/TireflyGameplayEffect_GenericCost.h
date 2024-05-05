// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "TireflyGameplayEffect_GenericCost.generated.h"


// 通用的能力消耗效果，结合 TireflyGameplayAbility 使用，它的属性资源消耗都可以在 TireflyGameplayAbility 的派生类中设置
UCLASS(DisplayName = "Generic Cost Effect")
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayEffect_GenericCost : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UTireflyGameplayEffect_GenericCost(const FObjectInitializer& ObjectInitializer);
};
