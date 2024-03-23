// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AsuraPlayerState.generated.h"

class UTireflyAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ASURAX_API AAsuraPlayerState : public APlayerState
	, public IAbilitySystemInterface
{
	GENERATED_BODY()

#pragma region PlayerState

public:
	AAsuraPlayerState(const FObjectInitializer& ObjectInitializer);

#pragma endregion


#pragma region GameplayAbilitySystem

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UTireflyAbilitySystemComponent* GetTireflyAbilitySystemComponent() const;

protected:
	UPROPERTY(Category = Ability, VisibleAnywhere, BlueprintReadOnly, Meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UTireflyAbilitySystemComponent> AbilitySystem;

#pragma endregion
};
