// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "TireflyExecCalc_AbilityCost.generated.h"


// 能力消耗的计算执行
UCLASS()
class TIREFLYGAMEPLAYABILITIES_API UTireflyExecCalc_AbilityCost : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
										FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
