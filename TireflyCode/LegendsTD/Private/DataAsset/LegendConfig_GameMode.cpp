// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "DataAsset/LegendConfig_GameMode.h"

#include "CoreBasics/CoreGameplayTypes.h"


ULegendConfig_GameMode::ULegendConfig_GameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

int32 ULegendConfig_GameMode::GetHeroPopulationOccupancy(EHeroRarity Rarity) const
{
	return HeroPopulationOccupancy.FindRef(Rarity);
}

int32 ULegendConfig_GameMode::GetHeroShopRefreshCost(EHeroRarity Rarity) const
{
	if (Rarity == EHeroRarity::Blue)
	{
		return BlueRefreshCost;
	}

	if (Rarity == EHeroRarity::Purple)
	{
		return PurpleRefreshCost;
	}

	return GoldRefreshCost;
}

const FVector& ULegendConfig_GameMode::GetHeroShopRefreshWeight(EHeroRarity Rarity) const
{
	if (Rarity == EHeroRarity::Blue)
	{
		return BlueRefreshWeight;
	}

	if (Rarity == EHeroRarity::Purple)
	{
		return PurpleRefreshWeight;
	}

	return GoldRefreshWeight;
}

int32 ULegendConfig_GameMode::GetHeroPurchasePrice(EHeroRarity Rarity) const
{
	if (Rarity == EHeroRarity::Blue)
	{
		return BlueHeroPurchasePrice;
	}

	if (Rarity == EHeroRarity::Purple)
	{
		return PurpleHeroPurchasePrice;
	}

	return GoldHeroPurchasePrice;
}

int32 ULegendConfig_GameMode::GetHeroSellingPrice(EHeroRarity Rarity, int32 Grade) const
{
	if (Rarity == EHeroRarity::Blue)
	{
		return BlueHeroSellingPrice.FindRef(Grade);
	}

	if (Rarity == EHeroRarity::Purple)
	{
		return PurpleHeroSellingPrice.FindRef(Grade);
	}

	return GoldHeroSellingPrice.FindRef(Grade);
}
