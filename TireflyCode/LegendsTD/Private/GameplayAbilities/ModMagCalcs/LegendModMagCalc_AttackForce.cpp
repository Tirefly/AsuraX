// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "GameplayAbilities/ModMagCalcs/LegendModMagCalc_AttackForce.h"

#include "Arena/CombatUnits/CombatUnitBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilities/LegendAttributeSet.h"
#include "Libraries/LegendSystemLibrary.h"


ULegendModMagCalc_AttackForce::ULegendModMagCalc_AttackForce()
{
	PhysiqueDef.AttributeToCapture = ULegendAttributeSet::GetPhysiqueAttribute();
	PhysiqueDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	PhysiqueDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(PhysiqueDef);

	AgilityDef.AttributeToCapture = ULegendAttributeSet::GetAgilityAttribute();
	AgilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	AgilityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(AgilityDef);

	WisdomDef.AttributeToCapture = ULegendAttributeSet::GetWisdomAttribute();
	WisdomDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	WisdomDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(WisdomDef);
}

float ULegendModMagCalc_AttackForce::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Coefficient;
	if (!ULegendSystemLibrary::GetDerivedAttributeCoefficient(ULegendAttributeSet::GetAttackForceAttribute(), Coefficient))
	{
		return 0.f;
	}

	const ACombatUnitBase* CombatUnit = Cast<ACombatUnitBase>(Spec.GetEffectContext().GetInstigator());
	if (!IsValid(CombatUnit))
	{
		return 0.f;
	}

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Physique;
	GetCapturedAttributeMagnitude(PhysiqueDef, Spec, EvaluationParameters, Physique);
	Physique = FMath::Max<float>(Physique, 0.f);

	float Agility;
	GetCapturedAttributeMagnitude(AgilityDef, Spec, EvaluationParameters, Agility);
	Agility = FMath::Max<float>(Agility, 0.f);

	float Wisdom;
	GetCapturedAttributeMagnitude(WisdomDef, Spec, EvaluationParameters, Wisdom);
	Wisdom = FMath::Max<float>(Wisdom, 0.f);

	float BaseAttribute = 0.f;
	switch (CombatUnit->GetMainAttributeType())
	{
		case EMainAttributeType::Physique:
		{
			BaseAttribute = Physique;
			break;
		}
		case EMainAttributeType::Agility:
		{
			BaseAttribute = Agility;
			break;
		}
		case EMainAttributeType::Wisdom:
		{
			BaseAttribute = Wisdom;
			break;
		}
	}

	return BaseAttribute * Coefficient;
}
