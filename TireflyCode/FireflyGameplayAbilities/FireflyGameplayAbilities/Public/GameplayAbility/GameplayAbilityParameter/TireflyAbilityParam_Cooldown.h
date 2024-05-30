// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "TireflyAbilityParam_Cooldown.generated.h"


class UFireflyAbilitySystemComponent;
class UTireflyAbilityParam_Numeric;


// GameplayAbility冷却时间设置的基础结构
UCLASS(Abstract)
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_CooldownBase : public UTireflyGameplayAbilityParameterBase
{
	GENERATED_BODY()

public:
	// 获取冷却时间标签
	UFUNCTION()
	virtual const FGameplayTagContainer& GetCooldownTags() const { return FGameplayTagContainer::EmptyContainer; }

	// 获取冷却时间
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	float GetCooldownDuration(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const;
	virtual float GetCooldownDuration_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const
	{
		return 0.f;
	}

	virtual bool IsShowcaseTextEditable_Implementation() const override { return false; }
};


// GameplayAbility通用的冷却时间参数
UCLASS(DisplayName = "Common Cooldown")
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_GenericCooldown : public UTireflyAbilityParam_CooldownBase
{
	GENERATED_BODY()

public:
	// 冷却时间标签
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer CooldownTags;

	// 冷却时间参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTireflyAbilityParam_Numeric* CooldownTime;

public:
	virtual const FGameplayTagContainer& GetCooldownTags() const override { return CooldownTags; }

	virtual float GetCooldownDuration_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;

	virtual FText GetShowcaseText_Implementation() const override;
};