// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TireflyGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

#pragma region GameplayAbility

public:
	UTireflyGameplayAbility(const FObjectInitializer& ObjectInitializer);

#pragma endregion
};
