// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "LegendModMagCalc_AttackSpeed.generated.h"


UCLASS()
class LEGENDSTD_API ULegendModMagCalc_AttackSpeed : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	ULegendModMagCalc_AttackSpeed();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	FGameplayEffectAttributeCaptureDefinition BaseAttributeDef;
};
