// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "LegendModMagCalc_DefensiveForce.generated.h"


UCLASS()
class LEGENDSTD_API ULegendModMagCalc_DefensiveForce : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	ULegendModMagCalc_DefensiveForce();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	FGameplayEffectAttributeCaptureDefinition BaseAttributeDef;
};
