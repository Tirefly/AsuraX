// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Cost.h"
#include "LegendAbilityParam_Cost.generated.h"


// 项目专用的技能花费
UCLASS(DisplayName = "通用技能花费")
class LEGENDSTD_API ULegendAbilityParam_Cost : public UTireflyAbilityParam_CostBase
{
	GENERATED_BODY()
	
public:
	// 获取花费的属性
	virtual FGameplayAttribute GetCostAttribute() const override;

	// 获取属性的修改方式
	virtual TEnumAsByte<EGameplayModOp::Type> GetModifierOp() const override;

	// 获取花费的数值
	virtual float GetCostValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;

	virtual bool CheckCost_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;
};
