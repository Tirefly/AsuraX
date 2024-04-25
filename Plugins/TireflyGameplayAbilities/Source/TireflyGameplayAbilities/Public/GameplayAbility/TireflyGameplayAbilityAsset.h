// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TireflyGameplayAbilityAsset.generated.h"


class UTireflyGameplayAbilityParameter;


/**
 * 
 */
UCLASS()
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbilityAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

#pragma region PrimaryDataAsset
	
public:
	UTireflyGameplayAbilityAsset(const FObjectInitializer& ObjectInitializer);

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	// 数据资产类型：能力资产
	static const FPrimaryAssetType GameplayAbilityAsset;

	// 能力的唯一标识Id
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayAbilityData)
	FName AbilityId = NAME_None;

#pragma endregion


#pragma region EditorDataProcess

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;	
#endif

#pragma endregion


#pragma region GameplayAbilityParameter

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GameplayAbilityParameters)
	TMap<FName, UTireflyGameplayAbilityParameter*> AbilityParameters;

#pragma endregion
};
