// Copyright Tirefly. All Rights Reserved.


#include "Framework/Character/AsuraBaseCharacter.h"

#include "TireflyAbilitySystemComponent.h"


// Sets default values
AAsuraBaseCharacter::AAsuraBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

}

// Called when the game starts or when spawned
void AAsuraBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* AAsuraBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

UTireflyAbilitySystemComponent* AAsuraBaseCharacter::GetTireflyAbilitySystemComponent() const
{
	return AbilitySystem;
}
