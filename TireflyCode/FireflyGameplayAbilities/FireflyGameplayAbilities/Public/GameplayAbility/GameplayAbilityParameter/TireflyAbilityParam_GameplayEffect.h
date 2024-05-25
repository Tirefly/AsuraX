// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffectTypes.h"
#include "TireflyAbilityParam_GameplayEffect.generated.h"


class UFireflyAbilitySystemComponent;
class UTireflyAbilityParam_Numeric;
class UGameplayEffect;


UCLASS(Abstract)
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_GameplayEffectBase : public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// 创建GameplayEffectSpec
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = Ability)
	FGameplayEffectSpecHandle MakeOutgoingGameplayEffectSpec(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo());
	virtual FGameplayEffectSpecHandle MakeOutgoingGameplayEffectSpec_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo())
	{
		return FGameplayEffectSpecHandle();
	}
	
	virtual bool IsShowcaseTextEditable_Implementation() const override { return false; }
};


// GameplayAbility的OtherGameplayEffectParam
UCLASS(DisplayName = "Other Gameplay Effect Param")
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_OtherGameplayEffectParam
	: public UTireflyAbilityParam_GameplayEffectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (GetOptions = "GetAbilityOtherGameplayEffectParameters"))
	FName ParameterName = NAME_None;

public:
	// 创建GameplayEffectSpec
	virtual FGameplayEffectSpecHandle MakeOutgoingGameplayEffectSpec_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) override;
};


// GameplayAbility的GameplayEffect运行时参数
UCLASS(DisplayName = "Gameplay Effect")
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_GameplayEffect
	: public UTireflyAbilityParam_GameplayEffectBase
{
	GENERATED_BODY()

public:
	// GameplayEffect类
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	// GameplayEffect的持续时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "IsEffectHasDuration", EditConditionHides))
	UTireflyAbilityParam_Numeric* DurationTime = nullptr;

	// GameplayEffect的周期间隔
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "IsEffectNotInstant", EditConditionHides))
	UTireflyAbilityParam_Numeric* PeriodTime = nullptr;

	// GameplayEffect的周期间隔
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "IsEffectNotInstant", EditConditionHides))
	UTireflyAbilityParam_Numeric* StackToApply = nullptr;

	// GameplayEffect的SetByCallerModifier
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier*> SetByCallerModifiers;

	// GameplayEffect的ContextSetting
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UTireflyAbilityParamDetail_GameplayEffect_ContextSetting*> ContextSettings;

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	// 创建GameplayEffectSpec
	virtual FGameplayEffectSpecHandle MakeOutgoingGameplayEffectSpec_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) override;

	UFUNCTION()
	bool IsEffectHasDuration() const;
	
	UFUNCTION()
	bool IsEffectNotInstant() const;
};


// GameplayEffect的参数细节
UCLASS(Abstract)
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect
	: public UTireflyGameplayAbilityParameterDetail
{
	GENERATED_BODY()
};


#pragma region SetByCallerDetail

// GameplayEffect的SetByCallerModifier设置细节基础结构
UCLASS(Abstract)
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier
	: public UTireflyAbilityParamDetail_GameplayEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTireflyAbilityParam_Numeric* Magnitude = nullptr;

public:
	// 为GameplayEffectSpec修改或添加SetByCallerModifier
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayEffect, Meta = (ForceAsFunction))
	void AssignSetByCallerModifier(UPARAM(ref)FGameplayEffectSpecHandle& Spec,
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo());
	virtual void AssignSetByCallerModifier_Implementation(UPARAM(ref)FGameplayEffectSpecHandle& EffectSpecHandle,
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) {}
};

// GameplayEffect的TagSetByCaller设置细节
UCLASS(DisplayName = "Tag Set By Caller")
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect_TagSetByCaller
	: public UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SetByCallerTag = FGameplayTag::EmptyTag;

public:
	// 为GameplayEffectSpec修改或添加SetByCallerModifier
	virtual void AssignSetByCallerModifier_Implementation(UPARAM(ref)FGameplayEffectSpecHandle& EffectSpecHandle,
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) override;
};

// GameplayEffect的TagSetByCaller设置细节
UCLASS(DisplayName = "Name Set By Caller")
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect_NameSetByCaller
	: public UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect_ContextSetting
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