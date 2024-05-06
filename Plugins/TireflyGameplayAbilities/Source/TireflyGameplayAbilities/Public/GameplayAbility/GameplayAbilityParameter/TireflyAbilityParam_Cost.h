// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffectTypes.h"
#include "TireflyAbilityParam_Cost.generated.h"


class UTireflyAbilitySystemComponent;
class UTireflyAbilityParam_Numeric;


// GameplayAbility花费设置的基础结构
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_CostBase : public UTireflyGameplayAbilityParameterBase
{
	GENERATED_BODY()

public:
	// 获取花费的属性
	UFUNCTION(BlueprintPure, Category = "Ability")
	virtual FGameplayAttribute GetCostAttribute() const { return FGameplayAttribute(nullptr); }

	// 获取属性的修改方式
	UFUNCTION(BlueprintPure, Category = "Ability")
	virtual TEnumAsByte<EGameplayModOp::Type> GetModifierOp() const { return EGameplayModOp::Additive; }

	// 获取花费的数值
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Ability")
	float GetCostValue(const UTireflyAbilitySystemComponent* CasterASC = nullptr,
		const UTireflyAbilitySystemComponent* TargetASC = nullptr,
		const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
		int32 Level = 1) const;
	virtual float GetCostValue_Implementation(const UTireflyAbilitySystemComponent* CasterASC = nullptr,
		const UTireflyAbilitySystemComponent* TargetASC = nullptr,
		const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
		int32 Level = 1) const
	{
		return 0.f;
	}

	// 检查是否满足花费
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Ability")
	bool CheckCost(const UTireflyAbilitySystemComponent* CasterASC = nullptr,
		const UTireflyAbilitySystemComponent* TargetASC = nullptr,
		const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
		int32 Level = 1) const;
	virtual bool CheckCost_Implementation(const UTireflyAbilitySystemComponent* CasterASC = nullptr,
		const UTireflyAbilitySystemComponent* TargetASC = nullptr,
		const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
		int32 Level = 1) const
	{
		return true;
	}
};


// GameplayAbility通用的花费设置
UCLASS(DisplayName = "Common Cost")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_CommonCost : public UTireflyAbilityParam_CostBase
{
	GENERATED_BODY()

public:
	// 花费的属性类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayAttribute Attribute = FGameplayAttribute(nullptr);

	// 属性的花费方式
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EGameplayModOp::Type> ModifierOp = EGameplayModOp::Additive;

	// 花费的值
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTireflyAbilityParam_Numeric* CostValue = nullptr;

public:
	virtual FGameplayAttribute GetCostAttribute() const override { return Attribute; }

	virtual TEnumAsByte<EGameplayModOp::Type> GetModifierOp() const override { return ModifierOp; }
	
	virtual float GetCostValue_Implementation(const UTireflyAbilitySystemComponent* CasterASC = nullptr,
		const UTireflyAbilitySystemComponent* TargetASC = nullptr,
		const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
		int32 Level = 1) const override;

	virtual bool CheckCost_Implementation(const UTireflyAbilitySystemComponent* CasterASC = nullptr,
		const UTireflyAbilitySystemComponent* TargetASC = nullptr,
		const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
		int32 Level = 1) const override;

	virtual FText GetShowcaseText_Implementation() const override;
};