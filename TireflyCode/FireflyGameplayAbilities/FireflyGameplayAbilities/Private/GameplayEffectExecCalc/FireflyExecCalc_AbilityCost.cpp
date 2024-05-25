// Copyright tzlFirefly, 2023. All Rights Reserved.


#include "GameplayEffectExecCalc/FireflyExecCalc_AbilityCost.h"

#include "FireflyAbilitySystemComponent.h"
#include "GameplayAbility/FireflyGameplayAbility.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Cost.h"


void UFireflyExecCalc_AbilityCost::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	auto TireflyASC = Cast<UFireflyAbilitySystemComponent>(ExecutionParams.GetTargetAbilitySystemComponent());
	if (!IsValid(TireflyASC))
	{
		return;
	}

	const UFireflyGameplayAbility* TireflyGA = Cast<UFireflyGameplayAbility>(
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

	FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo(TireflyASC,
		nullptr, AbilitySpec->Handle, AbilitySpec->Level);
	TArray<UTireflyAbilityParam_CostBase*> FinalCostSettings = TireflyGA->GetCostSettings();
	for (const UTireflyAbilityParam_CostBase* CostSetting : FinalCostSettings)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(CostSetting->GetCostAttribute(),
			CostSetting->GetModifierOp(), CostSetting->GetCostValue(ParamInfo)));
	}
}
