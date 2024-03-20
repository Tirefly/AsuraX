// Copyright tzlFirefly. All Rights Reserved.


#include "GameplayAbility/FireflyGameplayAbilityAsset.h"

const FPrimaryAssetType UFireflyGameplayAbilityAsset::GameplayAbilityAsset = TEXT("GameplayAbilityAsset");

UFireflyGameplayAbilityAsset::UFireflyGameplayAbilityAsset(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FPrimaryAssetId UFireflyGameplayAbilityAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(GameplayAbilityAsset, AbilityID);
}

TArray<FString> UFireflyGameplayAbilityAsset::GetAbilityParamNames() const
{
	TArray<FString> OutResult;
	for (const auto& Param : AbilityParameters)
	{
		OutResult.Add(Param.Key.ToString());
	}

	return OutResult;
}
