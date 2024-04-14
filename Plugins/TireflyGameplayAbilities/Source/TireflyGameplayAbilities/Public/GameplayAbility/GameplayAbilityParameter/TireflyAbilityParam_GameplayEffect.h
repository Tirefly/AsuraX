// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "GameplayTagContainer.h"
#include "TireflyAbilityParam_GameplayEffect.generated.h"


class UGameplayEffect;


// GameplayAbility的GameplayEffect运行时参数
UCLASS(DisplayName = "Gameplay Effect")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_GameplayEffect
	: public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// GameplayEffect类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EffectClass = nullptr;

	// GameplayEffect的持续时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "IsDurationParamNameNone()", EditConditionHides))
	float DurationTime = 0.f;

	// GameplayEffect的持续时间参数名称，用来指定Ability的其他参数作为该效果运行时的持续时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "DurationTime == 0", EditConditionHides, GetOptions = "GetAbilityParamOptions"))
	FName DurationParamName = NAME_None;

	// GameplayEffect的周期间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "IsPeriodParamNameNone()", EditConditionHides))
	float PeriodTime = 0.f;

	// GameplayEffect的周期间隔参数名称，用来指定Ability的其他参数作为该效果运行时的周期间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "PeriodTime == 0", EditConditionHides, GetOptions = "GetAbilityParamOptions"))
	FName PeriodParamName = NAME_None;

	// GameplayEffect的周期间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "IsStackToApplyParamNameNone()", EditConditionHides))
	int32 StackToApply = 1;

	// GameplayEffect的层数应用参数名称，用来指定Ability的其他参数作为该效果运行时的层数应用
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "StackToApply == 0", EditConditionHides, GetOptions = "GetAbilityParamOptions"))
	FName StackToApplyParamName = NAME_None;

	// GameplayEffect的SetByCallerModifier
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<class UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier*> SetByCallerModifiers;

	// GameplayEffect的ContextSetting
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<class UTireflyAbilityParamDetail_GameplayEffect_ContextSetting*> ContextSettings;

public:
	UFUNCTION()
	bool IsDurationParamNameNone() const { return DurationParamName.IsNone(); }

	UFUNCTION()
	bool IsPeriodParamNameNone() const { return PeriodParamName.IsNone(); }

	UFUNCTION()
	bool IsStackToApplyParamNameNone() const { return StackToApplyParamName.IsNone(); }
};


// Actor生成时附加的细节
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect
	: public UTireflyGameplayAbilityParameterDetail
{
	GENERATED_BODY()
};


// GameplayEffect的SetByCallerModifier
UCLASS(DisplayName = "Set By Caller Modifier")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier
	: public UTireflyAbilityParamDetail_GameplayEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SetByCallerTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTireflyAbilityParam_Numeric* Magnitude = nullptr;
};


// GameplayEffect的ContextSetting
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect_ContextSetting
	: public UTireflyAbilityParamDetail_GameplayEffect
{
	GENERATED_BODY()
};