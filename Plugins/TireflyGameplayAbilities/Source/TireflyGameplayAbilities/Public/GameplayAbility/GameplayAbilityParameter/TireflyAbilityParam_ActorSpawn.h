// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "TireflyAbilityParam_ActorSpawn.generated.h"


// 技能参数：Actor生成
UCLASS(DisplayName = "Actor Spawn")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_ActorSpawn : public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// 要生成的Actor类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AActor> ActorClass;

	// Actor生成时位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TObjectPtr<class UTireflyAbilityAssetElement_ActorSpawnTransform> SpawnTransform;

	// Actor生成时附加规则
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TObjectPtr<class UTireflyAbilityAssetElement_ActorAttachment> AttachmentRule;

	// Actor生成时碰撞处理方式
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpawnActorCollisionHandlingMethod SpawnCollisionHandling = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Actor生成时附加的细节
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<TObjectPtr<class UTireflyAbilityParamDetail_ActorSpawn>> SpawnDetails;

public:
	// 执行生成Actor
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability|Parameter", Meta = (WorldContext = "WorldContext",
		DeterminesOutputType = "InActorClass", AdvancedDisplay = "Owner, Instigator"))
	AActor* ExecuteActorSpawn(const UObject* WorldContext, TSubclassOf<AActor> InActorClass,
		FTransform InSpawnTransform, FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo(),
		AActor* Owner = nullptr, APawn* Instigator = nullptr);
	virtual AActor* ExecuteActorSpawn_Implementation(const UObject* WorldContext, TSubclassOf<AActor> InActorClass,
		FTransform InSpawnTransform, FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo(),
		AActor* Owner = nullptr, APawn* Instigator = nullptr);

	virtual bool IsDisplayTextEditable_Implementation() const override { return false; }
};


#pragma region Detail_ActorSpawn

// Actor生成时附加的细节
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParamDetail_ActorSpawn
	: public UTireflyGameplayAbilityParameterDetail
{
	GENERATED_BODY()

public:
	// Actor生成时附加细节
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability|Parameter")
	void HandleActorSpawned(AActor* SpawnedActor, FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo());
	virtual void HandleActorSpawned_Implementation(AActor* SpawnedActor, FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo())
	{}
};

#pragma endregion


#pragma region ActorSpawnTransform

// Actor生成时的生成位置细节
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityAssetElement_ActorSpawnTransform : public UTireflyAbilityAssetElement
{
	GENERATED_BODY()

public:
	// 获取Actor生成的位置数据
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability|Parameter")
	FTransform GetActorSpawnTransform(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo());
	virtual FTransform GetActorSpawnTransform_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo())
	{
		return FTransform();
	}
};


// Actor生成时的生成位置细节
UCLASS(DisplayName = "Actor Based Transform")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityAssetElement_ActorBasedTransform : public UTireflyAbilityAssetElement_ActorSpawnTransform
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
UCLASS(DisplayName = "Actor Attachment")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityAssetElement_ActorAttachment : public UTireflyAbilityAssetElement
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

	// 获取Actor生成时附加的规则
	FAttachmentTransformRules GetAttachmentTransformRules() const;

	// 处理Actor生成后的附加细节
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability|Parameter")
	void HandleActorAttached(AActor* SpawnedActor,
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo());
	virtual void HandleActorAttached_Implementation(AActor* SpawnedActor,
		FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo());
};

#pragma endregion