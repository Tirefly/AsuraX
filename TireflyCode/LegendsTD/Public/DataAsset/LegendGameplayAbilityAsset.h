// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/FireflyGameplayAbilityAsset.h"
#include "CoreBasics/CoreGameplayTypes.h"
#include "LegendGameplayAbilityAsset.generated.h"


class UFireflyGameplayAbility;


UCLASS()
class LEGENDSTD_API ULegendGameplayAbilityAsset : public UFireflyGameplayAbilityAsset
{
	GENERATED_BODY()

#pragma region PrimaryDataAsset

public:
	ULegendGameplayAbilityAsset(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

#pragma endregion


#pragma region CombatExtra

public:
	// 能力的视效数据
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AbilityBasics)
	FDataTableRowHandle AbilityCueHandleVFX;

#pragma endregion
};
