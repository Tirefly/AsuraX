// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "GameplayTagContainer.h"
#include "TireflyAbilityParam_Cooldown.generated.h"


class UTireflyAbilityParam_Numeric;


// GameplayAbility冷却时间设置的基础结构
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_CooldownBase : public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()
};


// GameplayAbility通用的冷却时间参数
UCLASS(DisplayName = "Common Cooldown")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_CommonCooldown : public UTireflyAbilityParam_CooldownBase
{
	GENERATED_BODY()

public:
	// 冷却时间标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer CooldownTags;

	// 冷却时间参数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTireflyAbilityParam_Numeric* CooldownTime;
};