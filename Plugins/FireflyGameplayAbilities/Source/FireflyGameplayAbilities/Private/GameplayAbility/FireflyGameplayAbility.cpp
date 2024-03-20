// Copyright tzlFirefly. All Rights Reserved.


#include "GameplayAbility/FireflyGameplayAbility.h"

#include "Engine/AssetManager.h"
#include "FireflyAbilitySystemComponent.h"
#include "FireflyAbilitySystemLibrary.h"
#include "GameplayAbility/FireflyGameplayAbilityAsset.h"

UFireflyGameplayAbility::UFireflyGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UFireflyGameplayAbility::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetName() == "AbilityID")
	{
		if (UAssetManager::Get().IsInitialized())
		{
			AbilityAsset = UAssetManager::Get().GetPrimaryAssetObject<UFireflyGameplayAbilityAsset>(
				FPrimaryAssetId(UFireflyGameplayAbilityAsset::GameplayAbilityAsset, AbilityID));
		}
	}
}

bool UFireflyGameplayAbility::GetParamSpecByAsset(FName ParamName, FInstancedStruct& OutParamSpec) const
{
	return UFireflyAbilitySystemLibrary::GetAbilityParamSpecOfAsset(AbilityAsset, ParamName, OutParamSpec
		, GetAbilityLevel(), GetAbilityCasterLevel());
}

bool UFireflyGameplayAbility::GetParamSpecByAssetAdvanced(FName ParamName, FInstancedStruct& OutParamSpec,
	int32 AbilityLevel, int32 CasterLevel) const
{
	return UFireflyAbilitySystemLibrary::GetAbilityParamSpecOfAsset(AbilityAsset, ParamName, OutParamSpec
		, AbilityLevel, CasterLevel);
}

int32 UFireflyGameplayAbility::GetAbilityCasterLevel() const
{
	const UFireflyAbilitySystemComponent* FireflyASC = Cast<UFireflyAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!IsValid(FireflyASC))
	{
		return -1;
	}

	return FireflyASC->GetAbilityActorLevel();
}
