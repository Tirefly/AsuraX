// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "GameplayTagContainer.h"
#include "TireflyAbilityParam_Cooldown.generated.h"


class UTireflyAbilitySystemComponent;
class UTireflyAbilityParam_Numeric;


// 处理技能冷却的技能资产元素
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_CooldownBase : public UTireflyAbilityAssetElement_DisplayText
{
	GENERATED_BODY()

public:
	// 冷却时间标签
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer CooldownTags;

public:
	// 获取冷却时间标签
	UFUNCTION()
	const FGameplayTagContainer& GetCooldownTags() const { return CooldownTags; }

	// 获取冷却时间
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Ability|Parameter")
	float GetCooldownDuration(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const;
	virtual float GetCooldownDuration_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const
	{
		return 0.f;
	}
};


// GameplayAbility通用的冷却时间设置
UCLASS(DisplayName = "Generic Cooldown")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_GenericCooldown : public UTireflyAbilityParam_CooldownBase
{
	GENERATED_BODY()

public:
	// 冷却时间参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TObjectPtr<UTireflyAbilityParam_Numeric> CooldownTime;

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;	
#endif

	virtual float GetCooldownDuration_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;

	virtual FText GetDisplayText_Implementation() const override;

	virtual bool IsDisplayTextEditable_Implementation() const override { return false; }
};