// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "Arena/GameFramework/ArenaAllianceCharacter.h"

#include "FireflyAbilitySystemComponent.h"


// Sets default values
AArenaAllianceCharacter::AArenaAllianceCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	AbilitySystem = CreateDefaultSubobject<UFireflyAbilitySystemComponent>(TEXT("AbilitySystem"));
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->PrimaryComponentTick.bCanEverTick = false;
		AbilitySystem->PrimaryComponentTick.bStartWithTickEnabled = false;
	}
}

// Called when the game starts or when spawned
void AArenaAllianceCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArenaAllianceCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystem->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* AArenaAllianceCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}
