// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AsuraBaseCharacter.generated.h"

class UTireflyAbilitySystemComponent;

UCLASS()
class ASURAX_API AAsuraBaseCharacter : public ACharacter
	, public IAbilitySystemInterface
{
	GENERATED_BODY()

#pragma region Character

public:
	// Sets default values for this character's properties
	AAsuraBaseCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
