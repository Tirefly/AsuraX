// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/TireflyGameplayAbility.h"

#include "Engine/AssetManager.h"
#include "GameplayAbility/TireflyGameplayAbilityAsset.h"


UTireflyGameplayAbility::UTireflyGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTireflyGameplayAbility::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetName() == "AbilityID")
	{
		if (AbilityId == NAME_None)
		{
			AbilityAsset = nullptr;
			AbilityAssetPath.Empty();
		}
		else
		{
			const UAssetManager& AssetManager = UAssetManager::Get();
			AbilityAsset = AssetManager.GetPrimaryAssetObject<UTireflyGameplayAbilityAsset>(
				FPrimaryAssetId(UTireflyGameplayAbilityAsset::GameplayAbilityAsset, AbilityId));
			if (AbilityAsset)
			{
				AbilityAssetPath = AbilityAsset.GetPathName();
			}
		}
	}
}

TArray<FName> UTireflyGameplayAbility::GetAbilityParamOptions() const
{
	TArray<FName> OutOptions;
	if (AbilityAsset)
	{
		AbilityAsset->AbilityParameters.GenerateKeyArray(OutOptions);
	}

	return OutOptions;
}

UTireflyGameplayAbilityParameter* UTireflyGameplayAbility::GetAbilityAssetParameter(FName ParamName) const
{
	if (AbilityAsset)
	{
		return AbilityAsset->AbilityParameters.FindRef(ParamName);
	}

	return nullptr;
}
