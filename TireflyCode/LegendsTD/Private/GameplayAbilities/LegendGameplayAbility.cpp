// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "GameplayAbilities/LegendGameplayAbility.h"

// #include "AbilitySystemBlueprintLibrary.h"
// #include "Arena/CombatExtra/CombatDomain.h"
// #include "Arena/CombatExtra/CombatProjectile.h"
#include "AbilitySystemGlobals.h"
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

void ULegendGameplayAbility::GetAbilityParamInfo(FTireflyAbilityParamInfo& ParamInfo)
{
	Super::GetAbilityParamInfo(ParamInfo);

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (IAllianceUnitInterface* AvatarUnit = Cast<IAllianceUnitInterface>(AvatarActor))
	{
		ParamInfo.TargetASC = Cast<UFireflyAbilitySystemComponent>(
			UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(
				AvatarUnit->GetCurrentTarget()));
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
		ULegendBuilder_CombatUnit* BuilderCU = CoreAS.GetPrimaryAssetObject<ULegendBuilder_CombatUnit>(
			FPrimaryAssetId(ULegendBuilder_CombatUnit::CombatUnitBuilder, FName(CombatUnitID)));
		if (!BuilderCU)
		{
			if (ACombatUnitBase* CombatUnit = Cast<ACombatUnitBase>(GetAvatarActorFromActorInfo()))
			{
				BuilderCU = CombatUnit->GetCombatUnitBuilder();
			}
		}
		
		LoadCombatUnitAbilityAsset(ThisAbilityID.IsEmpty() ? AbilityId : FName(ThisAbilityID), BuilderCU);
	}
}

void ULegendGameplayAbility::OnAbilityGranted_Implementation()
{
	Super::OnAbilityGranted_Implementation();

	LoadGameplayAbilityAsset();
}