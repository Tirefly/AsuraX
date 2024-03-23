// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Character/AsuraBaseCharacter.h"
#include "AsuraNonPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ASURAX_API AAsuraNonPlayerCharacter : public AAsuraBaseCharacter
{
	GENERATED_BODY()

#pragma region Character

public:
	AAsuraNonPlayerCharacter(const FObjectInitializer& ObjectInitializer);

#pragma endregion
};
