// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AsuraBaseCharacter.h"
#include "AsuraPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ASURAX_API AAsuraPlayerCharacter : public AAsuraBaseCharacter
{
	GENERATED_BODY()

#pragma region Character

public:
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

#pragma endregion
};
