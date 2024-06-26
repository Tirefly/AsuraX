// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "GameplayEffectTypes.h"
#include "TireflyAbilityParam_GameplayEffect.generated.h"


class UTireflyAbilitySystemComponent;
class UTireflyAbilityParam_Numeric;
class UGameplayEffect;


// 技能参数：GameplayEffect基类
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_GameplayEffectBase : public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// 创建GameplayEffectSpec
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Ability|Parameter")
	FGameplayEffectSpecHandle MakeOutgoingGameplayEffectSpec(
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo(), int32 Level = 1);
	virtual FGameplayEffectSpecHandle MakeOutgoingGameplayEffectSpec_Implementation(
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo(), int32 Level = 1)
	{
		return FGameplayEffectSpecHandle();
	}
	
	virtual bool IsDisplayTextEditable_Implementation() const override { return false; }
};


// 技能参数：基于GameplayEffectClass创建GameplayEffectSpec
UCLASS(DisplayName = "Gameplay Effect: Spec")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_GameplayEffectSpec
	: public UTireflyAbilityParam_GameplayEffectBase
{
	GENERATED_BODY()

public:
	// GameplayEffect类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	// GameplayEffect的持续时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Meta = (EditCondition = "IsEffectHasDuration", EditConditionHides))
	TObjectPtr<UTireflyAbilityParam_Numeric> DurationTime = nullptr;

	// GameplayEffect的周期间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Meta = (EditCondition = "IsEffectNotInstant", EditConditionHides))
	TObjectPtr<UTireflyAbilityParam_Numeric> PeriodTime = nullptr;

	// GameplayEffect的单次堆叠应用
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Meta = (EditCondition = "IsEffectNotInstant", EditConditionHides))
	TObjectPtr<UTireflyAbilityParam_Numeric> StackToApply = nullptr;

	// GameplayEffect的SetByCallerModifier
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<class UTireflyAbilityParamDetail_GameplayEffect_SetByCaller>> SetByCallers;

	// GameplayEffect的ContextSetting
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<class UTireflyAbilityParamDetail_GameplayEffect_ContextSetting>> ContextSettings;

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	// 创建GameplayEffectSpec
	virtual FGameplayEffectSpecHandle MakeOutgoingGameplayEffectSpec_Implementation(
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo(), int32 Level = 1) override;

	UFUNCTION()
	bool IsEffectHasDuration() const;
	
	UFUNCTION()
	bool IsEffectNotInstant() const;
};


UCLASS(DontCollapseCategories, DisplayName = "Gameplay Effect: Instance")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_GameplayEffectInstance
	: public UTireflyAbilityParam_GameplayEffectBase
{
	GENERATED_BODY()

public:
	// 能力的冷却时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTireflyGameplayEffect> GameplayEffect = nullptr;

	// GameplayEffect的ContextSetting
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<class UTireflyAbilityParamDetail_GameplayEffect_ContextSetting>> ContextSettings;

public:
	// 创建GameplayEffectSpec
	virtual FGameplayEffectSpecHandle MakeOutgoingGameplayEffectSpec_Implementation(
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo(), int32 Level = 1) override;
};


// GameplayAbility的OtherGameplayEffectParam
UCLASS(DisplayName = "Gameplay Effect: Another Param")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_AnotherGameplayEffectParam
	: public UTireflyAbilityParam_GameplayEffectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (GetOptions = "GetOtherGameplayEffectParameters"))
	FName ParameterName = NAME_None;

public:
	// 创建GameplayEffectSpec
	virtual FGameplayEffectSpecHandle MakeOutgoingGameplayEffectSpec_Implementation(
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo(), int32 Level = 1) override;
};


// GameplayEffect的参数细节
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect
	: public UTireflyGameplayAbilityParameterDetail
{
	GENERATED_BODY()
};


#pragma region SetByCallerDetail

// GameplayEffect的SetByCallerModifier设置细节基础结构
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect_SetByCaller
	: public UTireflyAbilityParamDetail_GameplayEffect
{
	GENERATED_BODY()

public:
	// SetByCallerModifier的参数值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TObjectPtr<UTireflyAbilityParam_Numeric> Magnitude = nullptr;

public:	
	// 为GameplayEffectSpec修改或添加SetByCallerModifier
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayEffect, Meta = (ForceAsFunction))
	void AssignSetByCallerModifier(UPARAM(ref)FGameplayEffectSpecHandle& EffectSpecHandle,
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo());
	virtual void AssignSetByCallerModifier_Implementation(UPARAM(ref)FGameplayEffectSpecHandle& EffectSpecHandle,
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) {}
};


// GameplayEffect的TagSetByCaller设置细节
UCLASS(DisplayName = "Tag Set By Caller")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect_TagSetByCaller
	: public UTireflyAbilityParamDetail_GameplayEffect_SetByCaller
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SetByCallerTag = FGameplayTag::EmptyTag;

public:
	// 为GameplayEffectSpec修改或添加SetByCallerModifier
	virtual void AssignSetByCallerModifier_Implementation(UPARAM(ref)FGameplayEffectSpecHandle& EffectSpecHandle,
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) override;
};


// GameplayEffect的TagSetByCaller设置细节
UCLASS(DisplayName = "Name Set By Caller")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect_NameSetByCaller
	: public UTireflyAbilityParamDetail_GameplayEffect_SetByCaller
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SetByCallerName = NAME_None;

public:
	// 为GameplayEffectSpec修改或添加SetByCallerModifier
	virtual void AssignSetByCallerModifier_Implementation(UPARAM(ref)FGameplayEffectSpecHandle& EffectSpecHandle,
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) override;
};

#pragma endregion


#pragma region GameplayEffectContextDetial

// GameplayEffect的ContextSetting
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect_ContextSetting
	: public UTireflyAbilityParamDetail_GameplayEffect
{
	GENERATED_BODY()

public:
	// 修改GameplayEffectContext
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayEffect, Meta = (ForceAsFunction))
	void ModifyGameplayEffectContext(UPARAM(ref)FGameplayEffectContextHandle& Context,
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo());
	virtual void ModifyGameplayEffectContext_Implementation(UPARAM(ref)FGameplayEffectContextHandle& Context,
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) {}
};

#pragma endregion