// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "LegendModMagCalc_AttackForce.generated.h"


UCLASS()
class LEGENDSTD_API ULegendModMagCalc_AttackForce : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	ULegendModMagCalc_AttackForce();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	FGameplayEffectAttributeCaptureDefinition PhysiqueDef;
	FGameplayEffectAttributeCaptureDefinition AgilityDef;
	FGameplayEffectAttributeCaptureDefinition WisdomDef;
};
