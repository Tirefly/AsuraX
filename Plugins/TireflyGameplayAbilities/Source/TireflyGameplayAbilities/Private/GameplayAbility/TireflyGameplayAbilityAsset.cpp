// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/TireflyGameplayAbilityAsset.h"


const FPrimaryAssetType UTireflyGameplayAbilityAsset::GameplayAbilityAsset = TEXT("GameplayAbilityAsset");


UTireflyGameplayAbilityAsset::UTireflyGameplayAbilityAsset(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FPrimaryAssetId UTireflyGameplayAbilityAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(GameplayAbilityAsset, AbilityID);
}

void UTireflyGameplayAbilityAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
