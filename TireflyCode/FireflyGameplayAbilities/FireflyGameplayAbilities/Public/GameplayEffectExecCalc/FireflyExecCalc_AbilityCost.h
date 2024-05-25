// Copyright tzlFirefly, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "FireflyExecCalc_AbilityCost.generated.h"


// 能力消耗的计算执行
// Execution Calculation of Ability 's cost
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyExecCalc_AbilityCost : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams
		, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};