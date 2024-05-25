// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "GameplayAbilities/GameplayEffects/LegendGameplayEffect_SummonInitializer.h"

#include "GameplayAbilities/ExecCalcs/LegendExecCalc_SummonInitializer.h"


ULegendGameplayEffect_SummonInitializer::ULegendGameplayEffect_SummonInitializer(
	const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
{
	{
		FGameplayEffectExecutionDefinition SummonInitializerExecCalc = FGameplayEffectExecutionDefinition();
		SummonInitializerExecCalc.CalculationClass = ULegendExecCalc_SummonInitializer::StaticClass();

		Executions.Add(SummonInitializerExecCalc);
	}
}
