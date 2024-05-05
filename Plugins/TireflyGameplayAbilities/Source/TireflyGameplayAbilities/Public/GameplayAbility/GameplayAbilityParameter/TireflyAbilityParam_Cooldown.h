// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "TireflyAbilityParam_Cooldown.generated.h"


class UTireflyAbilityParam_Numeric;


// GameplayAbility冷却时间设置的基础结构
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_CooldownBase : public UTireflyGameplayAbilityParameterBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual const FGameplayTagContainer& GetCooldownTags() const { return FGameplayTagContainer::EmptyContainer; }
	
	UFUNCTION()
	virtual float GetCooldownDuration(const UTireflyAbilitySystemComponent* CasterASC = nullptr,
									  const UTireflyAbilitySystemComponent* TargetASC = nullptr,
									  const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
									  int32 Level = 1) const
	{
		return 0.f;
	}
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

public:
	virtual const FGameplayTagContainer& GetCooldownTags() const override { return CooldownTags; }

	virtual float GetCooldownDuration(const UTireflyAbilitySystemComponent* CasterASC = nullptr,
	                                  const UTireflyAbilitySystemComponent* TargetASC = nullptr,
	                                  const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
	                                  int32 Level = 1) const override;
};