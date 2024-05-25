// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "LegendModMagCalc_ManaRecoveryOfAttack.generated.h"


UCLASS()
class LEGENDSTD_API ULegendModMagCalc_ManaRecoveryOfAttack : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	ULegendModMagCalc_ManaRecoveryOfAttack();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	FGameplayEffectAttributeCaptureDefinition BaseAttributeDef;
};
