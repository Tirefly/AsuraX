// Copyright Tirefly. All Rights Reserved.


#include "Framework/Character/AsuraNonPlayerCharacter.h"

#include "TireflyAbilitySystemComponent.h"


AAsuraNonPlayerCharacter::AAsuraNonPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystem = CreateDefaultSubobject<UTireflyAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}
