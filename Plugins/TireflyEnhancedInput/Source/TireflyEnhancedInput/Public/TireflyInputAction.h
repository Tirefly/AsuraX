// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameplayTagContainer.h"
#include "TireflyInputAction.generated.h"


UCLASS()
class TIREFLYENHANCEDINPUT_API UTireflyInputAction : public UInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Tag")
	FGameplayTag InputActionTag;
};
