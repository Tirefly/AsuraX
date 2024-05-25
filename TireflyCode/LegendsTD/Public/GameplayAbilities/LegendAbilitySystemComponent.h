// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireflyAbilitySystemComponent.h"
#include "LegendAbilitySystemComponent.generated.h"


UCLASS()
class LEGENDSTD_API ULegendAbilitySystemComponent : public UFireflyAbilitySystemComponent
{
	GENERATED_BODY()

#pragma region AbilitySystemComponent

public:
	ULegendAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);

#pragma endregion


#pragma region AbilityActorInfo

public:
	virtual int32 GetAbilityCasterLevel() const override;

#pragma endregion
};
