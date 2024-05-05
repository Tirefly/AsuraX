// Copyright Tirefly. All Rights Reserved.


#include "GameplayEffect/TireflyGameplayEffect_GenericCost.h"

#include "GameplayEffect/ExecCalc/TireflyExecCalc_AbilityCost.h"


UTireflyGameplayEffect_GenericCost::UTireflyGameplayEffect_GenericCost(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FGameplayEffectExecutionDefinition CostExecCalc = FGameplayEffectExecutionDefinition();
	CostExecCalc.CalculationClass = UTireflyExecCalc_AbilityCost::StaticClass();

	Executions.Add(CostExecCalc);
}
