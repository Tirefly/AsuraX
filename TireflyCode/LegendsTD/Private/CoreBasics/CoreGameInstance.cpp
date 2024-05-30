// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreBasics/CoreGameInstance.h"

#include "CoreBasics/CoreAssetManager.h"
#include "DataAsset/LegendBuilder_CombatUnit.h"
#include "DataAsset/LegendCombatBossConfig.h"
#include "DataAsset/LegendSynergyDataAsset.h"
#include "Kismet/KismetSystemLibrary.h"


UCoreGameInstance::UCoreGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCoreGameInstance::Init()
{
	Super::Init();

	UCoreAssetManager& CoreAM = UCoreAssetManager::Get();

	TArray<FPrimaryAssetId> CombatUnitBuildersToLoad;
	CoreAM.GetPrimaryAssetIdList(ULegendBuilder_CombatUnit::CombatUnitBuilder, CombatUnitBuildersToLoad);
	CoreAM.LoadPrimaryAssets(CombatUnitBuildersToLoad, TArray<FName>(), FStreamableDelegate::CreateUObject(
		this, &UCoreGameInstance::HandleCombatUnitBuildersLoaded, CombatUnitBuildersToLoad));

	TArray<FPrimaryAssetId> SynergyDataAssetsToLoad;
	CoreAM.GetPrimaryAssetIdList(ULegendSynergyDataAsset::SynergyDataAsset, SynergyDataAssetsToLoad);
	CoreAM.LoadPrimaryAssets(SynergyDataAssetsToLoad, TArray<FName>(), FStreamableDelegate::CreateUObject(
		this, &UCoreGameInstance::HandleSynergyDataAssetsLoaded, SynergyDataAssetsToLoad));
}

void UCoreGameInstance::HandleCombatUnitBuildersLoaded(TArray<FPrimaryAssetId> AssetsLoaded)
{
	const UCoreAssetManager& CoreAM = UCoreAssetManager::Get();

	for (const FPrimaryAssetId AssetId : AssetsLoaded)
	{
		if (!AssetId.IsValid())
		{
			continue;
		}

		// 检测构建器是否有效
		ULegendBuilder_CombatUnit* BuilderCU = CoreAM.GetPrimaryAssetObject<ULegendBuilder_CombatUnit>(AssetId);
		if (!IsValid(BuilderCU))
		{
			continue;
		}

		UKismetSystemLibrary::PrintString(this, BuilderCU->GetPrimaryAssetId().ToString(),
			true, true, FLinearColor::Black, 15);

		// 检测构建器核心数据是否有效
		if (!BuilderCU->CoreBuilderData.IsValid())
		{
			continue;
		}

		// 添加首领单位构建器
		if (IsValid(BuilderCU->BossConfig))
		{
			BossBuilders.Add(BuilderCU->GetPrimaryAssetId().PrimaryAssetName, BuilderCU);
		}

		// 添加英雄构建器
		if (const auto HeroData = BuilderCU->CoreBuilderData.GetPtr<FHeroBuilderData>())
		{
			switch (HeroData->Rarity)
			{
				case EHeroRarity::Blue:
				{
					// 蓝色英雄
					BlueHeroBuilders.Add(BuilderCU->GetPrimaryAssetId().PrimaryAssetName, BuilderCU);
					break;
				}
				case EHeroRarity::Purple:
				{
					// 紫色英雄
					PurpleHeroBuilders.Add(BuilderCU->GetPrimaryAssetId().PrimaryAssetName, BuilderCU);
					break;
				}
				case EHeroRarity::Gold:
				{
					// 金色英雄
					GoldHeroBuilders.Add(BuilderCU->GetPrimaryAssetId().PrimaryAssetName, BuilderCU);
					break;
				}
				default:
					break;
			}

			HeroBuilders.Add(BuilderCU->GetPrimaryAssetId().PrimaryAssetName, BuilderCU);
			continue;
		}

		// 添加小兵单位构建器
		if (BuilderCU->CoreBuilderData.GetScriptStruct() == FEnemyBuilderData::StaticStruct())
		{
			MinionBuilders.Add(BuilderCU->GetPrimaryAssetId().PrimaryAssetName, BuilderCU);
			continue;
		}

		// 添加召唤物单位构建器
		if (BuilderCU->CoreBuilderData.GetScriptStruct() == FSummonBuilderData::StaticStruct())
		{
			SummonBuilders.Add(BuilderCU->GetPrimaryAssetId().PrimaryAssetName, BuilderCU);
		}
	}
}

void UCoreGameInstance::HandleSynergyDataAssetsLoaded(TArray<FPrimaryAssetId> AssetsLoaded)
{
	const UCoreAssetManager& CoreAM = UCoreAssetManager::Get();

	for (const FPrimaryAssetId AssetId : AssetsLoaded)
	{
		if (!AssetId.IsValid())
		{
			continue;
		}

		// 检测构建器是否有效
		const ULegendSynergyDataAsset* SynergyDA = CoreAM.GetPrimaryAssetObject<ULegendSynergyDataAsset>(AssetId);
		if (!IsValid(SynergyDA))
		{
			continue;
		}

		UKismetSystemLibrary::PrintString(this, SynergyDA->GetPrimaryAssetId().ToString(), true, true, FLinearColor::Black, 15);
	}
}
