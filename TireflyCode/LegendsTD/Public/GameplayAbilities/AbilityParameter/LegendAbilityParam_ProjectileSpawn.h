// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyAbilityParam_ActorSpawn.h"
#include "LegendAbilityParam_ProjectileSpawn.generated.h"


UCLASS(DisplayName = "投射物子弹效果：命中目标时")
class LEGENDSTD_API UUTireflyAbilityParamDetail_ProjectileSpawn : public UTireflyAbilityParamDetail_ActorSpawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<class UTireflyAbilityParam_GameplayEffectBase*> Effects;

public:
	virtual void HandleActorSpawned_Implementation(AActor* SpawnedActor, FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) override;
};
