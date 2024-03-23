// Copyright Tirefly. All Rights Reserved.


#include "Framework/Character/AsuraPlayerCharacter.h"

#include "TireflyAbilitySystemComponent.h"
#include "Framework/Player/AsuraPlayerState.h"


void AAsuraPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AAsuraPlayerState* PS = GetPlayerState<AAsuraPlayerState>())
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystem = PS->GetTireflyAbilitySystemComponent();

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
}

void AAsuraPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AAsuraPlayerState* PS = GetPlayerState<AAsuraPlayerState>())
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystem = PS->GetTireflyAbilitySystemComponent();

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
}
