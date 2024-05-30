// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffectTypes.h"
#include "TireflyAbilityParam_Cost.generated.h"


class UFireflyAbilitySystemComponent;
class UTireflyAbilityParam_Numeric;


// GameplayAbility花费设置的基础结构
UCLASS(Abstract)
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_CostBase : public UTireflyGameplayAbilityParameterBase
{
	GENERATED_BODY()

public:
	// 获取花费的属性
	UFUNCTION(BlueprintPure)
	virtual FGameplayAttribute GetCostAttribute() const { return FGameplayAttribute(nullptr); }

	// 获取属性的修改方式
	UFUNCTION(BlueprintPure)
	virtual TEnumAsByte<EGameplayModOp::Type> GetModifierOp() const { return EGameplayModOp::Additive; }

	// 获取花费的数值
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	float GetCostValue(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const;
	virtual float GetCostValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const
	{
		return 0.f;
	}

	// 检查是否满足花费
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool CheckCost(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const;
	virtual bool CheckCost_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const
	{
		return true;
	}

	virtual bool IsShowcaseTextEditable_Implementation() const override { return false; }
};


// GameplayAbility通用的花费设置
UCLASS(DisplayName = "Common Cost")
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_GenericCost : public UTireflyAbilityParam_CostBase
{
	GENERATED_BODY()

public:
	// 花费的属性类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttribute Attribute = FGameplayAttribute(nullptr);

	// 属性的花费方式
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EGameplayModOp::Type> ModifierOp = EGameplayModOp::Additive;

	// 花费的值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTireflyAbilityParam_Numeric* CostValue = nullptr;

public:
	virtual FGameplayAttribute GetCostAttribute() const override { return Attribute; }

	virtual TEnumAsByte<EGameplayModOp::Type> GetModifierOp() const override { return ModifierOp; }
	
	virtual float GetCostValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;

	virtual bool CheckCost_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;

	virtual FText GetShowcaseText_Implementation() const override;
};