// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "GameplayEffectTypes.h"
#include "TireflyAbilityParam_Cost.generated.h"


class UTireflyAbilitySystemComponent;
class UTireflyAbilityParam_Numeric;


// 处理技能花费的技能资产元素
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_CostBase : public UTireflyAbilityAssetElement_DisplayText
{
	GENERATED_BODY()

public:	
	// 获取花费的属性
	UFUNCTION(BlueprintPure, Category = "Ability|Element")
	virtual FGameplayAttribute GetCostAttribute() const { return FGameplayAttribute(nullptr); }

	// 获取属性的修改方式
	UFUNCTION(BlueprintPure, Category = "Ability|Element")
	virtual TEnumAsByte<EGameplayModOp::Type> GetModifierOp() const { return EGameplayModOp::Additive; }

	// 获取花费的数值
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Ability|Element")
	float GetCostValue(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const;
	virtual float GetCostValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const
	{
		return 0.f;
	}

	// 检查是否满足花费
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Ability|Element")
	bool CheckCost(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const;
	virtual bool CheckCost_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const
	{
		return true;
	}
};


// GameplayAbility通用的花费设置
UCLASS(DisplayName = "Generic Cost")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_GenericCost : public UTireflyAbilityParam_CostBase
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

	virtual FText GetDisplayText_Implementation() const override;

	virtual bool IsDisplayTextEditable_Implementation() const override { return false; }
};