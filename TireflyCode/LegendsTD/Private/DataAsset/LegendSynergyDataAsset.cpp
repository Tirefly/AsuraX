// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "DataAsset/LegendSynergyDataAsset.h"

#include "DataAsset/LegendBuilder_CombatUnit.h"
#include "DataAsset/LegendConfig_CombatUnit.h"
#include "Libraries/LegendSystemLibrary.h"


const FPrimaryAssetType ULegendSynergyDataAsset::SynergyDataAsset = TEXT("SynergyDataAsset");


FPrimaryAssetId ULegendSynergyDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(SynergyDataAsset, SynergyID);
}

#if WITH_EDITOR
void ULegendSynergyDataAsset::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	if (SynergyUnitIDs.Num() > 0)
	{
		if (const ULegendConfig_CombatUnit* Config = ULegendSystemLibrary::GetCombatUnitConfig())
		{
			SynergyAttributeMagnitude = 0.f;
			for (const FName UnitID : SynergyUnitIDs)
			{
				if (const ULegendBuilder_CombatUnit* Builder = ULegendSystemLibrary::GetCombatUnitBuilder(UnitID))
				{
					if (const FHeroBuilderData* BuilderData = Builder->CoreBuilderData.GetPtr<FHeroBuilderData>())
					{
						if (Config->AttributeEnhancingSynergyMagnitude.Find(BuilderData->Rarity))
						{
							SynergyAttributeMagnitude += *Config->AttributeEnhancingSynergyMagnitude.Find(BuilderData->Rarity);
						}
					}
				}
			}
		}

		for (const FName UnitID : SynergyUnitIDs)
		{
			CombatUnitAbilitySystemConfigs.Add(UnitID);
		}
	}
}
#endif