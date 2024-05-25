// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "GameplayAbilities/AbilityParameter/LegendAbilityParam_DomainSpawn.h"

#include "Arena/CombatExtra/CombatDomain.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_GameplayEffect.h"


void UDomainSpawnDetail_EffectsToEnemiesOnEnter::HandleActorSpawned_Implementation(AActor* SpawnedActor,
	FTireflyAbilityParamInfo ParamInfo)
{
	ACombatDomain* Domain = Cast<ACombatDomain>(SpawnedActor);
	if (!Domain)
	{
		Super::HandleActorSpawned_Implementation(SpawnedActor, ParamInfo);
	}

	TArray<FGameplayEffectSpecHandle> EffectHandles;
	for (auto& Effect : Effects)
	{
		EffectHandles.Add(Effect->MakeOutgoingGameplayEffectSpec(ParamInfo));
	}

	Domain->DomainEffect.EffectsToEnemiesOnEnter = EffectHandles;
}

void UDomainSpawnDetail_EffectsToEnemiesOnExit::HandleActorSpawned_Implementation(AActor* SpawnedActor,
	FTireflyAbilityParamInfo ParamInfo)
{
	ACombatDomain* Domain = Cast<ACombatDomain>(SpawnedActor);
    if (!Domain)
    {
    	Super::HandleActorSpawned_Implementation(SpawnedActor, ParamInfo);
    }
    
    TArray<FGameplayEffectSpecHandle> EffectHandles;
    for (auto& Effect : Effects)
    {
    	EffectHandles.Add(Effect->MakeOutgoingGameplayEffectSpec(ParamInfo));
    }
    
    Domain->DomainEffect.EffectsToEnemiesOnExit = EffectHandles;
}

void UDomainSpawnDetail_EffectsToEnemiesOnDestruction::HandleActorSpawned_Implementation(AActor* SpawnedActor,
	FTireflyAbilityParamInfo ParamInfo)
{
	ACombatDomain* Domain = Cast<ACombatDomain>(SpawnedActor);
	if (!Domain)
	{
		Super::HandleActorSpawned_Implementation(SpawnedActor, ParamInfo);
	}
    
	TArray<FGameplayEffectSpecHandle> EffectHandles;
	for (auto& Effect : Effects)
	{
		EffectHandles.Add(Effect->MakeOutgoingGameplayEffectSpec(ParamInfo));
	}
    
	Domain->DomainEffect.EffectsToEnemiesOnDestruction = EffectHandles;
}

void UDomainSpawnDetail_EffectsToAlliesOnEnter::HandleActorSpawned_Implementation(AActor* SpawnedActor,
	FTireflyAbilityParamInfo ParamInfo)
{
	ACombatDomain* Domain = Cast<ACombatDomain>(SpawnedActor);
	if (!Domain)
	{
		Super::HandleActorSpawned_Implementation(SpawnedActor, ParamInfo);
	}
    
	TArray<FGameplayEffectSpecHandle> EffectHandles;
	for (auto& Effect : Effects)
	{
		EffectHandles.Add(Effect->MakeOutgoingGameplayEffectSpec(ParamInfo));
	}
    
	Domain->DomainEffect.EffectsToAlliesOnEnter = EffectHandles;
}

void UDomainSpawnDetail_EffectsToAlliesOnExit::HandleActorSpawned_Implementation(AActor* SpawnedActor,
	FTireflyAbilityParamInfo ParamInfo)
{
	ACombatDomain* Domain = Cast<ACombatDomain>(SpawnedActor);
	if (!Domain)
	{
		Super::HandleActorSpawned_Implementation(SpawnedActor, ParamInfo);
	}
    
	TArray<FGameplayEffectSpecHandle> EffectHandles;
	for (auto& Effect : Effects)
	{
		EffectHandles.Add(Effect->MakeOutgoingGameplayEffectSpec(ParamInfo));
	}
    
	Domain->DomainEffect.EffectsToAlliesOnExit = EffectHandles;
}

void UDomainSpawnDetail_EffectsToAlliesOnDestruction::HandleActorSpawned_Implementation(AActor* SpawnedActor,
	FTireflyAbilityParamInfo ParamInfo)
{
	ACombatDomain* Domain = Cast<ACombatDomain>(SpawnedActor);
	if (!Domain)
	{
		Super::HandleActorSpawned_Implementation(SpawnedActor, ParamInfo);
	}
    
	TArray<FGameplayEffectSpecHandle> EffectHandles;
	for (auto& Effect : Effects)
	{
		EffectHandles.Add(Effect->MakeOutgoingGameplayEffectSpec(ParamInfo));
	}
    
	Domain->DomainEffect.EffectsToAlliesOnDestruction = EffectHandles;
}
