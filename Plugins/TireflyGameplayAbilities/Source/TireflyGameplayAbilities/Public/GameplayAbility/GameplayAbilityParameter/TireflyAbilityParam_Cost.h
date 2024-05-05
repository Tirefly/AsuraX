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
	UFUNCTION()
	virtual FGameplayAttribute GetCostAttribute() const { return FGameplayAttribute(nullptr); }

	UFUNCTION()
	virtual TEnumAsByte<EGameplayModOp::Type> GetModifierOp() const { return EGameplayModOp::Additive; }
	
	UFUNCTION()
	virtual float GetCostValue(const UTireflyAbilitySystemComponent* CasterASC = nullptr,
							   const UTireflyAbilitySystemComponent* TargetASC = nullptr,
							   const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
							   int32 Level = 1) const
	{
		return 0.f;
	}

	UFUNCTION()
	virtual bool CheckCost(const UTireflyAbilitySystemComponent* CasterASC = nullptr,
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
	
	virtual float GetCostValue(const UTireflyAbilitySystemComponent* CasterASC = nullptr,
							   const UTireflyAbilitySystemComponent* TargetASC = nullptr,
							   const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
							   int32 Level = 1) const override;

	virtual bool CheckCost(const UTireflyAbilitySystemComponent* CasterASC = nullptr,
						   const UTireflyAbilitySystemComponent* TargetASC = nullptr,
						   const FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle(),
						   int32 Level = 1) const override;
};