// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyAbilityParam_ActorSpawn.h"
#include "LegendAbilityParam_DomainSpawn.generated.h"


class ACombatDomain;


UCLASS(Abstract)
class LEGENDSTD_API UTireflyAbilityParamDetail_DomainSpawn : public UTireflyAbilityParamDetail_ActorSpawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UTireflyAbilityParam_GameplayEffectBase*> Effects;
};


UCLASS(DisplayName = "领域光环Buff：敌人进入领域时")
class LEGENDSTD_API UDomainSpawnDetail_EffectsToEnemiesOnEnter : public UTireflyAbilityParamDetail_DomainSpawn
{
	GENERATED_BODY()

public:
	virtual void HandleActorSpawned_Implementation(AActor* SpawnedActor, FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) override;
};


UCLASS(DisplayName = "领域光环Buff：敌人离开领域时")
class LEGENDSTD_API UDomainSpawnDetail_EffectsToEnemiesOnExit : public UTireflyAbilityParamDetail_DomainSpawn
{
	GENERATED_BODY()

public:
	virtual void HandleActorSpawned_Implementation(AActor* SpawnedActor, FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) override;
};


UCLASS(DisplayName = "领域光环Buff：领域消失时对敌人")
class LEGENDSTD_API UDomainSpawnDetail_EffectsToEnemiesOnDestruction : public UTireflyAbilityParamDetail_DomainSpawn
{
	GENERATED_BODY()

public:
	virtual void HandleActorSpawned_Implementation(AActor* SpawnedActor, FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) override;
};


UCLASS(DisplayName = "领域光环Buff：友方进入领域时")
class LEGENDSTD_API UDomainSpawnDetail_EffectsToAlliesOnEnter : public UTireflyAbilityParamDetail_DomainSpawn
{
	GENERATED_BODY()

public:
	virtual void HandleActorSpawned_Implementation(AActor* SpawnedActor, FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) override;
};


UCLASS(DisplayName = "领域光环Buff：友方离开领域时")
class LEGENDSTD_API UDomainSpawnDetail_EffectsToAlliesOnExit : public UTireflyAbilityParamDetail_DomainSpawn
{
	GENERATED_BODY()

public:
	virtual void HandleActorSpawned_Implementation(AActor* SpawnedActor, FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) override;
};


UCLASS(DisplayName = "领域光环Buff：领域消失时对友方")
class LEGENDSTD_API UDomainSpawnDetail_EffectsToAlliesOnDestruction : public UTireflyAbilityParamDetail_DomainSpawn
{
	GENERATED_BODY()
	
public:
	virtual void HandleActorSpawned_Implementation(AActor* SpawnedActor, FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) override;
};
