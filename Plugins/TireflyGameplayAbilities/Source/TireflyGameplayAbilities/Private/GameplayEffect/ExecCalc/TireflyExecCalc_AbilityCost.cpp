// Copyright Tirefly. All Rights Reserved.


#include "GameplayEffect/ExecCalc/TireflyExecCalc_AbilityCost.h"

#include "TireflyAbilitySystemComponent.h"
#include "GameplayAbility/TireflyGameplayAbility.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Cost.h"


void UTireflyExecCalc_AbilityCost::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	auto TireflyASC = Cast<UTireflyAbilitySystemComponent>(ExecutionParams.GetTargetAbilitySystemComponent());
	if (!IsValid(TireflyASC))
	{
		return;
	}

	const UTireflyGameplayAbility* TireflyGA = Cast<UTireflyGameplayAbility>(
		ExecutionParams.GetOwningSpec().GetEffectContext().GetAbilityInstance_NotReplicated());
	if (!IsValid(TireflyGA))
	{
		return;
	}

	FGameplayAbilitySpec* AbilitySpec = TireflyASC->FindAbilitySpecFromClass(TireflyGA->GetClass());
	if (!AbilitySpec)
	{
		return;
	}

	if (UTireflyAbilityParam_CostBase* FinalCostSetting = TireflyGA->GetCostSetting())
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(FinalCostSetting->GetCostAttribute(),
			FinalCostSetting->GetModifierOp(), FinalCostSetting->GetCostValue(TireflyGA->GetAbilityParamInfo())));
	}
}
