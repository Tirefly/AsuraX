// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffectTypes.h"
#include "TireflyAbilityParam_GameplayEffect.generated.h"


class UTireflyAbilitySystemComponent;
class UTireflyAbilityParam_Numeric;
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
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	// GameplayEffect的持续时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "IsEffectHasDuration", EditConditionHides))
	UTireflyAbilityParam_Numeric* DurationTime = nullptr;

	// GameplayEffect的周期间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "IsEffectNotInstant", EditConditionHides))
	UTireflyAbilityParam_Numeric* PeriodTime = nullptr;

	// GameplayEffect的周期间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "IsEffectNotInstant", EditConditionHides))
	UTireflyAbilityParam_Numeric* StackToApply = nullptr;

	// GameplayEffect的SetByCallerModifier
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<class UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier*> SetByCallerModifiers;

	// GameplayEffect的ContextSetting
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<class UTireflyAbilityParamDetail_GameplayEffect_ContextSetting*> ContextSettings;

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	// 创建GameplayEffectSpec
	UFUNCTION(BlueprintPure, Category = Ability)
	virtual FGameplayEffectSpecHandle MakeOutgoingGameplayEffectSpec(UTireflyAbilitySystemComponent* CasterASC = nullptr,
																	 const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
																	 int32 Level = 1);

	UFUNCTION()
	bool IsEffectHasDuration() const;
	
	UFUNCTION()
	bool IsEffectNotInstant() const;
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
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier
	: public UTireflyAbilityParamDetail_GameplayEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "Magnitude == nullptr", EditConditionHides, GetOptions = "GetSetByCallerModifierParamNames"))
	FName MagnitudeName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "ShouldHideMagnitude", EditConditionHides))
	UTireflyAbilityParam_Numeric* Magnitude = nullptr;

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION()
	bool ShouldHideMagnitude() const { return MagnitudeName == NAME_None; }

	UFUNCTION()
	TArray<FName> GetSetByCallerModifierParamNames() const;
	
	// 为GameplayEffectSpec修改或添加SetByCallerModifier
	UFUNCTION(BlueprintNativeEvent, Category = GameplayEffect, Meta = (ForceAsFunction))
	void AssignSetByCallerModifier(UPARAM(ref)FGameplayEffectSpecHandle& Spec,
								  UTireflyAbilitySystemComponent* CasterASC = nullptr,
								  const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
								  int32 Level = 1);
	virtual void AssignSetByCallerModifier_Implementation(UPARAM(ref)FGameplayEffectSpecHandle& EffectSpecHandle,
														  UTireflyAbilitySystemComponent* CasterASC = nullptr,
														  const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
														  int32 Level = 1) {}

	UTireflyAbilityParam_Numeric* GetMagnitude() const;
};

// GameplayEffect的TagSetByCaller设置细节
UCLASS(DisplayName = "Tag Set By Caller")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect_TagSetByCaller
	: public UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SetByCallerTag = FGameplayTag::EmptyTag;

public:
	// 为GameplayEffectSpec修改或添加SetByCallerModifier
	virtual void AssignSetByCallerModifier_Implementation(UPARAM(ref)FGameplayEffectSpecHandle& EffectSpecHandle,
														  UTireflyAbilitySystemComponent* CasterASC = nullptr,
														  const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
														  int32 Level = 1) override;
};

// GameplayEffect的TagSetByCaller设置细节
UCLASS(DisplayName = "Name Set By Caller")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_GameplayEffect_NameSetByCaller
	: public UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SetByCallerName = NAME_None;

public:
	// 为GameplayEffectSpec修改或添加SetByCallerModifier
	virtual void AssignSetByCallerModifier_Implementation(UPARAM(ref)FGameplayEffectSpecHandle& EffectSpecHandle,
														  UTireflyAbilitySystemComponent* CasterASC = nullptr,
														  const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
														  int32 Level = 1) override;
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
	UFUNCTION(BlueprintNativeEvent, Category = GameplayEffect, Meta = (ForceAsFunction))
	void ModifyGameplayEffectContext(UPARAM(ref)FGameplayEffectContextHandle& Context,
									 UTireflyAbilitySystemComponent* CasterASC = nullptr,
					   				 const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
					   				 int32 Level = 1);
	virtual void ModifyGameplayEffectContext_Implementation(UPARAM(ref)FGameplayEffectContextHandle& Context,
															UTireflyAbilitySystemComponent* CasterASC = nullptr,
															const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
															int32 Level = 1){}
};

#pragma endregion