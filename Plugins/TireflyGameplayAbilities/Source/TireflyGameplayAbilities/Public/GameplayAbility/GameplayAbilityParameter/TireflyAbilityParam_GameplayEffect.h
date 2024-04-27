// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "GameplayTagContainer.h"
#include "TireflyAbilityParam_GameplayEffect.generated.h"


class UGameplayEffect;
class UTireflyAbilityParam_Numeric;


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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTireflyAbilityParam_Numeric* DurationTime = nullptr;

	// GameplayEffect的周期间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTireflyAbilityParam_Numeric* PeriodTime = nullptr;

	// GameplayEffect的周期间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTireflyAbilityParam_Numeric* StackToApply = nullptr;

	// GameplayEffect的SetByCallerModifier
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<class UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier*> SetByCallerModifiers;

	// GameplayEffect的ContextSetting
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<class UTireflyAbilityParamDetail_GameplayEffect_ContextSetting*> ContextSettings;
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