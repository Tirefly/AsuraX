// Copyright Tirefly. All Rights Reserved.


#include "Framework/Player/AsuraPlayerState.h"

#include "TireflyAbilitySystemComponent.h"


AAsuraPlayerState::AAsuraPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	AbilitySystem = CreateDefaultSubobject<UTireflyAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* AAsuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

UTireflyAbilitySystemComponent* AAsuraPlayerState::GetTireflyAbilitySystemComponent() const
{
	return AbilitySystem;
}
