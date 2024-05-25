// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyGameplayAbilityParameter.h"
#include "TireflyAbilityParam_ActorSpawn.generated.h"


#pragma region Param_ActorSpawn

// GameplayAbility的Actor生成参数
UCLASS(DisplayName = "Actor Spawn")
class LEGENDSTD_API UTireflyAbilityParam_ActorSpawn : public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// 要生成的Actor类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorClass;

	// 要生成的Actor类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ActorId;

	// 是否使用对象池
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseObjectPool = true;

	// 是否使用对象池
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "bUseObjectPool", EditConditionHides))
	class UTireflyAbilityParam_Numeric* LifetimeForObjectPool;

	// Actor生成时位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTireflyAbilityParamDetail_SpawnTransform* SpawnTransform;

	// Actor生成时附加规则
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTireflyAbilityParamDetail_ActorAttachment* AttachmentRule;

	// Actor生成时碰撞处理方式
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpawnActorCollisionHandlingMethod SpawnCollisionHandling = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Actor生成时附加的细节
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UTireflyAbilityParamDetail_ActorSpawn*> SpawnDetails;

public:
	// 生成Actor
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability", Meta = (WorldContext = "WorldContext",
		DeterminesOutputType = "InActorClass", AdvancedDisplay = "Owner, Instigator"))
	AActor* ExecuteActorSpawn(const UObject* WorldContext, TSubclassOf<AActor> InActorClass,
		FTransform InSpawnTransform, FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo(),
		AActor* Owner = nullptr, APawn* Instigator = nullptr);
	virtual AActor* ExecuteActorSpawn_Implementation(const UObject* WorldContext, TSubclassOf<AActor> InActorClass,
		FTransform InSpawnTransform, FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo(),
		AActor* Owner = nullptr, APawn* Instigator = nullptr);

	virtual bool IsShowcaseTextEditable_Implementation() const override { return false; }
};

#pragma endregion


#pragma region Detail_ActorSpawn

// Actor生成时附加的细节
UCLASS(Abstract)
class LEGENDSTD_API UTireflyAbilityParamDetail_ActorSpawn
	: public UTireflyGameplayAbilityParameterDetail
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability")
	void HandleActorSpawned(AActor* SpawnedActor, FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo());
	virtual void HandleActorSpawned_Implementation(AActor* SpawnedActor, FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo())
	{}
};

#pragma endregion


#pragma region ActorSpawnTransform

// Actor生成时的生成位置细节
UCLASS(Abstract, CollapseCategories, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class LEGENDSTD_API UTireflyAbilityParamDetail_SpawnTransform : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability")
	FTransform GetActorSpawnTransform(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo());
	virtual FTransform GetActorSpawnTransform_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo())
	{
		return FTransform();
	}
};


// Actor生成时的生成位置细节
UCLASS(DisplayName = "Actor Based Transform")
class LEGENDSTD_API UActorSpawnTransformDetail_ActorBasedTransform : public UTireflyAbilityParamDetail_SpawnTransform
{
	GENERATED_BODY()

public:
	// Actor生成时依据的源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETireflyAbilityParamSourceType SpawnSource = ETireflyAbilityParamSourceType::Caster;

	// Actor生成时附加的骨骼
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SpawnBaseBone = NAME_None;

	// Actor生成时相对于源的偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform OffsetFromSource;

public:
	virtual FTransform GetActorSpawnTransform_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) override;
};

#pragma endregion


#pragma region ActorSpawnAttachment

// Actor生成时的生成位置细节
UCLASS(CollapseCategories, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew, DisplayName = "Actor Attachment")
class LEGENDSTD_API UTireflyAbilityParamDetail_ActorAttachment : public UObject
{
	GENERATED_BODY()

public:
	// Actor生成时依据的源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETireflyAbilityParamSourceType AttachSource = ETireflyAbilityParamSourceType::Caster;

	// Actor生成时附加的骨骼
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttachBone = NAME_None;

	// Actor生成时相对于源的偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform AttachOffset;
	
	// Actor生成时附加的规则
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (GetOptions = "GetActorSpawnAttachmentRuleOptions"))
	FName AttachmentRule = FName("Keep Relative Transform");

public:
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

	FAttachmentTransformRules GetAttachmentTransformRules() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability")
	void HandleActorAttached(AActor* SpawnedActor,
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo());
	virtual void HandleActorAttached_Implementation(AActor* SpawnedActor,
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo());
};

#pragma endregion