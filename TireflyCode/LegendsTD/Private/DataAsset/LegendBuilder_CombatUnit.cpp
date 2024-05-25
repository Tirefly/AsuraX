// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/LegendBuilder_CombatUnit.h"

#include "FireflyGameplayAbilitiesSettings.h"


const FPrimaryAssetType ULegendBuilder_CombatUnit::CombatUnitBuilder = TEXT("CombatUnitBuilder");


FPrimaryAssetId ULegendBuilder_CombatUnit::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(CombatUnitBuilder, CombatUnitID);
}

void ULegendBuilder_CombatUnit::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!PropertyChangedEvent.MemberProperty)
	{
		return;
	}

	if (PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ULegendBuilder_CombatUnit, CombatUnitID))
	{
		if (CombatUnitID == NAME_None)
		{
			return;
		}

		const UFireflyGameplayAbilitiesSettings* Settings = GetDefault<UFireflyGameplayAbilitiesSettings>();
		if (!Settings)
		{
			return;
		}

		FCombatUnitBuilderData* CoreData = CoreBuilderData.GetMutablePtr<FCombatUnitBuilderData>();
		if (!CoreData)
		{
			return;
		}

		CoreData->CombatUnitName = FText::FromStringTable(Settings->GetGameplayNameLocalizationTableId(CombatUnitID), CombatUnitID.ToString());
		CoreData->CombatUnitDescription = FText::FromStringTable(Settings->GetGameplayDescriptionLocalizationTableId(CombatUnitID), CombatUnitID.ToString());
	}
}

ULegendGameplayAbilityAsset* ULegendBuilder_CombatUnit::GetSkillConfigAtGrade(int32 Grade) const
{
	switch (Grade)
	{
	case 1:
	{
		return SkillConfigs.IsValidIndex(0) ? SkillConfigs[0] : nullptr;
	}
	case 3:
	{
		return SkillConfigs.IsValidIndex(1) ? SkillConfigs[1] : nullptr;
	}
	case 5:
	{
		return SkillConfigs.IsValidIndex(2) ? SkillConfigs[2] : nullptr;
	}
	default:
		break;
	}

	return nullptr;
}
