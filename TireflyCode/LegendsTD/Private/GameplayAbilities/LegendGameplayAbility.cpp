// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "GameplayAbilities/LegendGameplayAbility.h"

// #include "AbilitySystemBlueprintLibrary.h"
// #include "Arena/CombatExtra/CombatDomain.h"
// #include "Arena/CombatExtra/CombatProjectile.h"
#include "Arena/CombatUnits/CombatUnitBase.h"
#include "CoreBasics/CoreAssetManager.h"
#include "DataAsset/LegendBuilder_CombatUnit.h"
#include "DataAsset/LegendGameplayAbilityAsset.h"


ULegendGameplayAbility::ULegendGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULegendGameplayAbility::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetName() == FString("AbilityId")
		|| PropertyChangedEvent.Property->GetName() == FString("AbilityType"))
	{
		FString AbilityAssetId = AbilityId.ToString();
		if (AbilityAssetId == "None" || AbilityType == ELegendAbilityType::None)
		{
			AbilityAsset = nullptr;
			AbilityAssetPath.Empty();
			return;
		}
		
		UCoreAssetManager& CoreAS = UCoreAssetManager::Get();

		// 战斗单位能力
		if (AbilityType == ELegendAbilityType::CombatUnitAbility)
		{
			FString CombatUnitID, ThisAbilityID;
			AbilityAssetId.Split(TEXT("."), &CombatUnitID, &ThisAbilityID);
			const ULegendBuilder_CombatUnit* CombatUnitBuilder = CoreAS.GetPrimaryAssetObject<ULegendBuilder_CombatUnit>(
				FPrimaryAssetId(ULegendBuilder_CombatUnit::CombatUnitBuilder, FName(CombatUnitID)));
			if (!CombatUnitBuilder)
			{
				return;
			}

			// 普通攻击
			if (IsValid(CombatUnitBuilder->NormalAttackConfig))
			{
				if (CombatUnitBuilder->NormalAttackConfig->AbilityID == FName(ThisAbilityID))
				{
					AbilityAsset = CombatUnitBuilder->NormalAttackConfig;
					AbilityAssetPath = AbilityAsset->GetPathName();

					return;
				}
			}

			// 技能
			for (auto Asset : CombatUnitBuilder->SkillConfigs)
			{
				if (!IsValid(Asset))
				{
					continue;
				}

				if (Asset->AbilityID != FName(ThisAbilityID))
				{
					continue;
				}

				AbilityAsset = Asset;
				AbilityAssetPath = AbilityAsset->GetPathName();
			}
		}
	}
}

void ULegendGameplayAbility::LoadGameplayAbilityAsset()
{
	FString AbilityAssetId = AbilityId.ToString();
	if (AbilityAssetId == "None" || AbilityType == ELegendAbilityType::None)
	{
		AbilityAsset = nullptr;
		AbilityAssetPath.Empty();
		return;
	}
		
	UCoreAssetManager& CoreAS = UCoreAssetManager::Get();

	auto LoadCombatUnitAbilityAsset = [this](FName InAbilityId, const ULegendBuilder_CombatUnit* CombatUnitBuilder)
	{
		if (!CombatUnitBuilder)
		{
			return;
		}

		if (IsValid(CombatUnitBuilder->NormalAttackConfig))
		{
			if (CombatUnitBuilder->NormalAttackConfig->AbilityID == InAbilityId)
			{
				AbilityAsset = CombatUnitBuilder->NormalAttackConfig;
				AbilityAssetPath = AbilityAsset->GetPathName();

				return;
			}
		}

		for (auto Asset : CombatUnitBuilder->SkillConfigs)
		{
			if (!IsValid(Asset))
			{
				continue;
			}

			if (Asset->AbilityID != InAbilityId)
			{
				continue;
			}

			AbilityAsset = Asset;
			AbilityAssetPath = AbilityAsset->GetPathName();
		}
	};

	// 战斗单位能力
	if (AbilityType == ELegendAbilityType::None)
	{
		if (ACombatUnitBase* CombatUnit = Cast<ACombatUnitBase>(GetAvatarActorFromActorInfo()))
		{
			LoadCombatUnitAbilityAsset(AbilityId, CombatUnit->GetCombatUnitBuilder());
		}

		return;
	}
	
	if (AbilityType == ELegendAbilityType::CombatUnitAbility)
	{
		FString CombatUnitID, ThisAbilityID;
		AbilityAssetId.Split(TEXT("."), &CombatUnitID, &ThisAbilityID);
		LoadCombatUnitAbilityAsset(FName(ThisAbilityID), CoreAS.GetPrimaryAssetObject<ULegendBuilder_CombatUnit>(
			FPrimaryAssetId(ULegendBuilder_CombatUnit::CombatUnitBuilder, FName(CombatUnitID))));
	}
}

void ULegendGameplayAbility::OnAbilityGranted_Implementation()
{
	Super::OnAbilityGranted_Implementation();

	LoadGameplayAbilityAsset();
}

// bool ULegendGameplayAbility::ConstructProjectileRuntimeFromConfig(TSubclassOf<ACombatProjectile>& ProjectileClass,
// 	FName& ProjectileID, FCombatExtraSkin& ProjectileSkin, FCombatProjectileEffect& ProjectileEffect, int32 Index)
// {
// 	const ULegendGameplayAbilityAsset* AssetGA = GetGameplayAbilityAsset<ULegendGameplayAbilityAsset>();
// 	if (!IsValid(AssetGA))
// 	{
// 		return false;
// 	}
//
// 	if (!AssetGA->ProjectileDefinition.IsValidIndex(Index))
// 	{
// 		return false;
// 	}
//
// 	ProjectileClass = AssetGA->ProjectileDefinition[Index].ProjectileClass;
// 	ProjectileID = AssetGA->ProjectileDefinition[Index].ProjectileID;
// 	ProjectileSkin.ExtraMesh = AssetGA->ProjectileDefinition[Index].ProjectileSkin.ExtraMesh;
// 	ProjectileSkin.ExtraMeshSkin = AssetGA->ProjectileDefinition[Index].ProjectileSkin.ExtraMeshSkin;
// 	ProjectileSkin.ExtraNS = AssetGA->ProjectileDefinition[Index].ProjectileSkin.ExtraNS;
//
// 	for (const int32 EffectIndex : AssetGA->ProjectileDefinition[Index].EffectsToTarget)
// 	{
// 		if (!AssetGA->EffectConfigs.IsValidIndex(EffectIndex))
// 		{
// 			continue;
// 		}
//
// 		ProjectileEffect.EffectsToTarget.Emplace(FGameplayEffectSpecHandle(MakeOutgoingGameplayEffectSpecByAsset(EffectIndex)));
// 	}
//
// 	return true;
// }
//
// bool ULegendGameplayAbility::ConstructDomainRuntimeFromConfig(TSubclassOf<ACombatDomain>& DomainClass, FName& DomainID,
// 	FCombatExtraSkin& DomainSkin, FCombatDomainEffect& DomainEffect, int32 Index)
// {
// 	const ULegendGameplayAbilityAsset* AssetGA = GetGameplayAbilityAsset<ULegendGameplayAbilityAsset>();
// 	if (!IsValid(AssetGA))
// 	{
// 		return false;
// 	}
//
// 	if (!AssetGA->DomainDefinition.IsValidIndex(Index))
// 	{
// 		return false;
// 	}
//
// 	// 从能力资产中获取域类、域ID和域皮肤信息
// 	DomainClass = AssetGA->DomainDefinition[Index].DomainClass;
// 	DomainID = AssetGA->DomainDefinition[Index].DomainID;
// 	DomainSkin.ExtraMesh = AssetGA->DomainDefinition[Index].DomainSkin.ExtraMesh;
// 	DomainSkin.ExtraMeshSkin = AssetGA->DomainDefinition[Index].DomainSkin.ExtraMeshSkin;
// 	DomainSkin.ExtraNS = AssetGA->DomainDefinition[Index].DomainSkin.ExtraNS;
//
// 	// 为敌人进入域时的效果列表添加效果
// 	DomainEffect.EffectsToEnemiesOnEnter.Empty();
// 	for (const int32 EffectIndex : AssetGA->DomainDefinition[Index].EffectsToEnemiesOnEnter)
// 	{
// 		if (!AssetGA->EffectConfigs.IsValidIndex(EffectIndex))
// 		{
// 			continue;
// 		}
//
// 		DomainEffect.EffectsToEnemiesOnEnter.Emplace(FGameplayEffectSpecHandle(MakeOutgoingGameplayEffectSpecByAsset(EffectIndex)));
// 	}
//
// 	// 为敌人退出域时的效果列表添加效果
// 	DomainEffect.EffectsToEnemiesOnExit.Empty();
// 	for (const int32 EffectIndex : AssetGA->DomainDefinition[Index].EffectsToEnemiesOnExit)
// 	{
// 		if (!AssetGA->EffectConfigs.IsValidIndex(EffectIndex))
// 		{
// 			continue;
// 		}
//
// 		DomainEffect.EffectsToEnemiesOnExit.Emplace(FGameplayEffectSpecHandle(MakeOutgoingGameplayEffectSpecByAsset(EffectIndex)));
// 	}
//
// 	// 为敌人域销毁时的效果列表添加效果
// 	DomainEffect.EffectsToEnemiesOnDestruction.Empty();
// 	for (const int32 EffectIndex : AssetGA->DomainDefinition[Index].EffectsToEnemiesOnDestruction)
// 	{
// 		if (!AssetGA->EffectConfigs.IsValidIndex(EffectIndex))
// 		{
// 			continue;
// 		}
//
// 		DomainEffect.EffectsToEnemiesOnDestruction.Emplace(FGameplayEffectSpecHandle(MakeOutgoingGameplayEffectSpecByAsset(EffectIndex)));
// 	}
//
// 	// 为盟友进入域时的效果列表添加效果
// 	DomainEffect.EffectsToAlliesOnEnter.Empty();
// 	for (const int32 EffectIndex : AssetGA->DomainDefinition[Index].EffectsToAlliesOnEnter)
// 	{
// 		if (!AssetGA->EffectConfigs.IsValidIndex(EffectIndex))
// 		{
// 			continue;
// 		}
//
// 		DomainEffect.EffectsToAlliesOnEnter.Emplace(FGameplayEffectSpecHandle(MakeOutgoingGameplayEffectSpecByAsset(EffectIndex)));
// 	}
//
// 	// 为盟友退出域时的效果列表添加效果
// 	DomainEffect.EffectsToAlliesOnExit.Empty();
// 	for (const int32 EffectIndex : AssetGA->DomainDefinition[Index].EffectsToAlliesOnExit)
// 	{
// 		if (!AssetGA->EffectConfigs.IsValidIndex(EffectIndex))
// 		{
// 			continue;
// 		}
//
// 		DomainEffect.EffectsToAlliesOnExit.Emplace(FGameplayEffectSpecHandle(MakeOutgoingGameplayEffectSpecByAsset(EffectIndex)));
// 	}
//
// 	// 为盟友域销毁时的效果列表添加效果
// 	DomainEffect.EffectsToAlliesOnDestruction.Empty();
// 	for (const int32 EffectIndex : AssetGA->DomainDefinition[Index].EffectsToAlliesOnDestruction)
// 	{
// 		if (!AssetGA->EffectConfigs.IsValidIndex(EffectIndex))
// 		{
// 			continue;
// 		}
//
// 		DomainEffect.EffectsToAlliesOnDestruction.Emplace(FGameplayEffectSpecHandle(MakeOutgoingGameplayEffectSpecByAsset(EffectIndex)));
// 	}
//
// 	return true;
// }