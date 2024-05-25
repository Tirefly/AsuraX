// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "GameplayAbility/FireflyGameplayAbilityAsset.h"

#include "FireflyGameplayAbilitiesSettings.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Description.h"


const FPrimaryAssetType UFireflyGameplayAbilityAsset::GameplayAbilityAsset = TEXT("GameplayAbilityAsset");


UFireflyGameplayAbilityAsset::UFireflyGameplayAbilityAsset(const FObjectInitializer& ObjectInitializer)
{
}

FPrimaryAssetId UFireflyGameplayAbilityAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(GameplayAbilityAsset, AbilityID);
}

void UFireflyGameplayAbilityAsset::PostInitProperties()
{
	Super::PostInitProperties();

	K2_PostInitProperties();
}

#if WITH_EDITOR
void UFireflyGameplayAbilityAsset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!PropertyChangedEvent.MemberProperty)
	{
		return;
	}

	if (PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UFireflyGameplayAbilityAsset, AbilityID))
	{
		if (AbilityID == NAME_None)
		{
			return;
		}

		const UFireflyGameplayAbilitiesSettings* Settings = GetDefault<UFireflyGameplayAbilitiesSettings>();
		if (!Settings)
		{
			return;
		}

		AbilityName = FText::FromStringTable(Settings->GetGameplayNameLocalizationTableId(AbilityID), AbilityID.ToString());
		if (UTireflyAbilityParam_CommonDescription* Description = Cast<UTireflyAbilityParam_CommonDescription>(AbilityDescription))
		{
			Description->ShowcaseText = FText::FromStringTable(Settings->GetGameplayDescriptionLocalizationTableId(AbilityID), AbilityID.ToString());
		}
	}
}
#endif
