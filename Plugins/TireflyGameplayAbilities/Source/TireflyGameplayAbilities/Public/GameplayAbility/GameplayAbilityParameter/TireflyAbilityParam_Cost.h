// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "GameplayEffectTypes.h"
#include "TireflyAbilityParam_Cost.generated.h"


class UTireflyAbilityParam_Numeric;


// GameplayAbility花费设置的基础结构
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_CostBase : public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTireflyAbilityParam_Numeric* CostValue = nullptr;
};