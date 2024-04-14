// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "TireflyAbilityParam_ActorSpawn.generated.h"


// GameplayAbility的Actor生成参数
UCLASS(DisplayName = "Actor Spawn")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_ActorSpawn : public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// 要生成的Actor类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> ActorClass;

	// Actor生成时依据的源
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (GetOptions = "GetActorSpawnSourceOptions"))
	FName SpawnSource = FName("Caster");

	// Actor生成时相对于源的偏移
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTransform OffsetFromSource;

	// Actor生成时碰撞处理方式
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESpawnActorCollisionHandlingMethod SpawnCollisionHandling = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	// Actor生成时附加的骨骼
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SpawnAttachBone = NAME_None;

	// Actor生成时附加的规则
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (GetOptions = "GetActorSpawnAttachmentRuleOptions"))
	FName AttachmentRule = FName("Keep Relative Transform");

	// Actor生成时附加的细节
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<class UTireflyAbilityParamDetail_ActorSpawn*> SpawnDetails;

public:
	UFUNCTION()
	static TArray<FName> GetActorSpawnSourceOptions() { return TArray<FName>{"Caster", "Target" }; }

	UFUNCTION()
	static TArray<FName> GetActorSpawnAttachmentRuleOptions()
	{
		return TArray<FName>{
			"Keep Relative Transform",
			"Keep World Transform",
			"Snap To Target Not Including Scale",
			"Snap To Target Including Scale"
		};
	}
};


// Actor生成时附加的细节
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_ActorSpawn
	: public UTireflyGameplayAbilityParameterDetail
{
	GENERATED_BODY()
};