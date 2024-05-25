// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "GameplayAbilities/AbilityParameter/LegendAbilityParam_ProjectileSpawn.h"

#include "Arena/CombatExtra/CombatProjectile.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_GameplayEffect.h"


void UUTireflyAbilityParamDetail_ProjectileSpawn::HandleActorSpawned_Implementation(AActor* SpawnedActor,
                                                                                    FTireflyAbilityParamInfo ParamInfo)
{
	ACombatProjectile* Domain = Cast<ACombatProjectile>(SpawnedActor);
	if (!Domain)
	{
		Super::HandleActorSpawned_Implementation(SpawnedActor, ParamInfo);
	}

	TArray<FGameplayEffectSpecHandle> EffectHandles;
	for (auto& Effect : Effects)
	{
		EffectHandles.Add(Effect->MakeOutgoingGameplayEffectSpec(ParamInfo));
	}

	Domain->ProjectileEffect.EffectsToTarget = EffectHandles;
}
