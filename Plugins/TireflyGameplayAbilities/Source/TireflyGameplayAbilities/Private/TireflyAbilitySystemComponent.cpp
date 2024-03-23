// Copyright Tirefly. All Rights Reserved.


#include "TireflyAbilitySystemComponent.h"

#include "AbilitySystemLog.h"

UTireflyAbilitySystemComponent::UTireflyAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTireflyAbilitySystemComponent::K2_InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		UE_LOG(LogAbilitySystem, Warning, TEXT("UTireflyAbilitySystemComponent::K2_InitAbilityActorInfo: Can't call this from a simulated proxy actor."));		return;
		return;
	}
	
	InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}
