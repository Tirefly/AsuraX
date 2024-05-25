// Copyright tzlFirefly, 2023. All Rights Reserved.


#include "GameplayEffect/FireflyGameplayEffect_GenericCost.h"

#include "GameplayEffectExecCalc/FireflyExecCalc_AbilityCost.h"


UFireflyGameplayEffect_GenericCost::UFireflyGameplayEffect_GenericCost(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FGameplayEffectExecutionDefinition CostExecCalc = FGameplayEffectExecutionDefinition();
	CostExecCalc.CalculationClass = UFireflyExecCalc_AbilityCost::StaticClass();

	Executions.Add(CostExecCalc);
}
