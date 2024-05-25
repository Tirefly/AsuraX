// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "GameplayAbilities/ModMagCalcs/LegendModMagCalc_ManaRecoveryOfAttack.h"

#include "GameplayAbilities/LegendAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Libraries/LegendSystemLibrary.h"


ULegendModMagCalc_ManaRecoveryOfAttack::ULegendModMagCalc_ManaRecoveryOfAttack()
{
	BaseAttributeDef.AttributeToCapture = ULegendAttributeSet::GetPhysiqueAttribute();
	BaseAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	BaseAttributeDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(BaseAttributeDef);
}

float ULegendModMagCalc_ManaRecoveryOfAttack::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Coefficient;
	if (!ULegendSystemLibrary::GetDerivedAttributeCoefficient(ULegendAttributeSet::GetManaRecoveryOfAttackAttribute(), Coefficient))
	{
		return 0.f;
	}

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float BaseAttribute;
	GetCapturedAttributeMagnitude(BaseAttributeDef, Spec, EvaluationParameters, BaseAttribute);
	BaseAttribute = FMath::Max<float>(BaseAttribute, 0.f);

	return BaseAttribute * Coefficient;
}
