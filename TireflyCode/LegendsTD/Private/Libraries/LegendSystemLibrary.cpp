// Fill out your copyright notice in the Description page of Project Settings.


#include "Libraries/LegendSystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Arena/GameFramework/ArenaGameMode.h"
#include "Arena/GameFramework/ArenaPlayerController.h"
#include "Arena/GameFramework/ArenaPlayerState.h"
#include "Arena/CombatUnits/CombatUnitBase.h"
#include "AttributeSet.h"

#include "CoreBasics/CoreAssetManager.h"
#include "CoreBasics/CoreGameInstance.h"
#include "CoreBasics/CoreGameplayTypes.h"
#include "DataAsset/LegendConfig_CombatUnit.h"
#include "DataAsset/LegendBuilder_CombatUnit.h"

#include "Kismet/GameplayStatics.h"
#include "FireflyAlgorithmLibrary.h"
#include "FireflyGridMapBase.h"
#include "FireflyGridMapLibrary.h"
#include "FireflyGridMovementComponent.h"
#include "Arena/CombatUnits/CombatUnit_Summon.h"
#include "DataAsset/LegendConfig_GameMode.h"
#include "DataAsset/LegendSynergyDataAsset.h"
#include "GameplayAbilities/LegendAttributeSet.h"


UCoreGameInstance* ULegendSystemLibrary::GetCoreGameInstance(const UObject* WorldContextObject)
{
	return Cast<UCoreGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
}

AArenaGameMode* ULegendSystemLibrary::GetArenaGameMode(const UObject* WorldContextObject)
{
	return Cast<AArenaGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
}

ULegendConfig_GameMode* ULegendSystemLibrary::GetGameModeConfig(const UObject* WorldContextObject)
{
	if (const AArenaGameMode* ArenaGM = GetArenaGameMode(WorldContextObject))
	{
		return ArenaGM->GetGameModeConfig();
	}

	return nullptr;
}

AArenaPlayerController* ULegendSystemLibrary::GetArenaPlayerController(const UObject* WorldContextObject,
                                                                       int32 PlayerIndex)
{
	return Cast<AArenaPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, PlayerIndex));
}

AArenaPlayerState* ULegendSystemLibrary::GetArenaPlayerState(const UObject* WorldContextObject, int32 PlayerIndex)
{
	return Cast<AArenaPlayerState>(UGameplayStatics::GetPlayerState(WorldContextObject, PlayerIndex));
}

void ULegendSystemLibrary::AddEnemyFormatToDataTable(UDataTable* DataTable, const FName& Row, const FLegendEnemyFormatTableRow& Format)
{
	DataTable->AddRow(Row, Format);
	DataTable->MarkPackageDirty();
}

const ULegendConfig_CombatUnit* ULegendSystemLibrary::GetCombatUnitConfig()
{
	const UCoreAssetManager& CoreAM = UCoreAssetManager::Get();
	if (!CoreAM.IsValid())
	{
		return nullptr;
	}

	return CoreAM.GetCombatUnitConfig();
}

const ULegendConfig_EnemyFormat* ULegendSystemLibrary::GetEnemyFormatConfig()
{
	const UCoreAssetManager& CoreAM = UCoreAssetManager::Get();
	if (!CoreAM.IsValid())
	{
		return nullptr;
	}

	return CoreAM.GetEnemyFormatConfig();
}

const ULegendSynergyDataAsset* ULegendSystemLibrary::GetSynergyDataAsset(FName SynergyID)
{
	const UCoreAssetManager& CoreAM = UCoreAssetManager::Get();

	return CoreAM.GetPrimaryAssetObject<ULegendSynergyDataAsset>(FPrimaryAssetId(
		ULegendSynergyDataAsset::SynergyDataAsset, SynergyID));
}

bool ULegendSystemLibrary::GetDerivedAttributeCoefficient(const FGameplayAttribute& Attribute, float& OutCoefficient)
{
	const ULegendConfig_CombatUnit* CombatUnitConfig = GetCombatUnitConfig();
	if (!IsValid(CombatUnitConfig))
	{
		return false;
	}

	if (!CombatUnitConfig->DerivedAttributeCoefficients.Find(Attribute))
	{
		return false;
	}

	OutCoefficient = CombatUnitConfig->DerivedAttributeCoefficients[Attribute];
	return true;
}

float ULegendSystemLibrary::CalculateAttackPlayRate(float AttackSpeed)
{
	const ULegendConfig_CombatUnit* CombatUnitConfig = GetCombatUnitConfig();
	if (!IsValid(CombatUnitConfig))
	{
		return 1.f;
	}

	return AttackSpeed / FMath::Max<double>((CombatUnitConfig->AttackSpeedCoefficients.X * CombatUnitConfig->AttackSpeedCoefficients.Y), 0.00001f);
}

float ULegendSystemLibrary::CalculateAttackPerSecond(float AttackSpeed)
{
	const ULegendConfig_CombatUnit* CombatUnitConfig = GetCombatUnitConfig();
	if (!IsValid(CombatUnitConfig))
	{
		return 1.f;
	}

	return (CombatUnitConfig->AttackSpeedCoefficients.X * CombatUnitConfig->AttackSpeedCoefficients.Y) / FMath::Max<double>(AttackSpeed, 0.00001f);
}

ULegendBuilder_CombatUnit* ULegendSystemLibrary::GetCombatUnitBuilder(FName CombatUnitID)
{
	const UCoreAssetManager& CoreAM = UCoreAssetManager::Get();

	return CoreAM.GetPrimaryAssetObject<ULegendBuilder_CombatUnit>(FPrimaryAssetId(
		ULegendBuilder_CombatUnit::CombatUnitBuilder, CombatUnitID));
}

void ULegendSystemLibrary::GetHeroBuildersByRarity(const UObject* WorldContext, EHeroRarity HeroRarity,
	TMap<FName, ULegendBuilder_CombatUnit*>& OutBuilders)
{
	const UCoreGameInstance* CoreGI = GetCoreGameInstance(WorldContext);
	if (!IsValid(CoreGI))
	{
		return;
	}

	switch (HeroRarity)
	{
		case EHeroRarity::Blue:
		{
			OutBuilders = CoreGI->BlueHeroBuilders;
			break;
		}
		case EHeroRarity::Purple:
		{
			OutBuilders = CoreGI->PurpleHeroBuilders;
			break;
		}
		case EHeroRarity::Gold:
		{
			OutBuilders = CoreGI->GoldHeroBuilders;
			break;
		}
		case EHeroRarity::Max:
		{
			break;
		}
	}
}

void ULegendSystemLibrary::GetMinionBuilders(const UObject* WorldContext, TMap<FName, ULegendBuilder_CombatUnit*>& OutBuilders)
{
	const UCoreGameInstance* CoreGI = GetCoreGameInstance(WorldContext);
	if (!IsValid(CoreGI))
	{
		return;
	}

	OutBuilders = CoreGI->MinionBuilders;
}

void ULegendSystemLibrary::GetBossBuilders(const UObject* WorldContext, TMap<FName, ULegendBuilder_CombatUnit*>& OutBuilders)
{
	const UCoreGameInstance* CoreGI = GetCoreGameInstance(WorldContext);
	if (!IsValid(CoreGI))
	{
		return;
	}

	OutBuilders = CoreGI->BossBuilders;
}

AActor* ULegendSystemLibrary::GetNearestOtherAllianceTarget(const UObject* WorldContextObject, AActor* InActor)
{
	const AArenaGameMode* ArenaGM = GetArenaGameMode(InActor);
	if (!IsValid(ArenaGM))
	{
		return nullptr;
	}

	ACombatUnitBase* CombatUnit = Cast<ACombatUnitBase>(InActor);
	if (!IsValid(CombatUnit))
	{
		return nullptr;
	}

	TArray<AActor*> PendingTargets = TArray<AActor*>{};
	for (const auto Container : ArenaGM->GetAllianceUnitContainers())
	{
		if (Container.Key == CombatUnit->GetCurrentAlliance())
		{
			continue;
		}

		TArray<AActor*> CurrentUnits = Container.Value.AllianceAliveUnits;
		CurrentUnits = UFireflyAlgorithmLibrary::ActorArray_SortByDistance(CurrentUnits, CombatUnit->GetActorLocation());
		PendingTargets.Append(CurrentUnits);
	}

	PendingTargets = UFireflyAlgorithmLibrary::ActorArray_SortByDistance(PendingTargets, CombatUnit->GetActorLocation());
	for (int32 i = 0; i < PendingTargets.Num(); ++i)
	{
		if (const ACombatUnitBase* PendingUnit = Cast<ACombatUnitBase>(PendingTargets[i]))
		{
			if (PendingUnit->CanBeTargeted(CombatUnit))
			{
				return PendingTargets[i];
			}
		}
	}

	return nullptr;
}

TArray<AActor*> ULegendSystemLibrary::GetAliveAllies(const UObject* WorldContextObject, AActor* InActor)
{
	const AArenaGameMode* ArenaGM = GetArenaGameMode(InActor);
	if (!IsValid(ArenaGM))
	{
		return TArray<AActor*>{};
	}

	const ACombatUnitBase* CU = Cast<ACombatUnitBase>(InActor);
	if (!IsValid(CU))
	{
		return TArray<AActor*>{};
	}

	const AController* Alliance = CU->GetCurrentAlliance();
	if (!IsValid(Alliance))
	{
		return TArray<AActor*>{};
	}

	auto AllianceContainers = ArenaGM->GetAllianceUnitContainers();
	if (!AllianceContainers.Find(Alliance))
	{
		return TArray<AActor*>{};
	}

	return AllianceContainers[Alliance].AllianceAliveUnits;
}

float ULegendSystemLibrary::GetPriceDealingInHero(EHeroRarity HeroRarity, bool IsBuy)
{
	float OutResult = 0.f;
	TArray<float> OutResults;
	ULL_JsonDataManager::GetJsonDataManager()->GetJsonDataFromKey(
		IsBuy ? TEXT("HeroPool.PriceBuyHero") : TEXT("HeroPool.PriceSellHero"),
		OutResult, OutResults);

	UEnum* EnumHR = StaticEnum<EHeroRarity>();
	if (!EnumHR)
	{
		return OutResult;
	}
	if (OutResults.Num() < EnumHR->GetMaxEnumValue())
	{
		return OutResult;
	}

	int32 Index = (int32)HeroRarity;
	return OutResults[Index];
}

float ULegendSystemLibrary::GetPriceDealingInEquipment(bool IsBuy)
{
	float OutResult = 0.f;
	TArray<float> OutResults;
	ULL_JsonDataManager::GetJsonDataManager()->GetJsonDataFromKey(
		IsBuy ? TEXT("Shop.PriceBuyEquipment") : TEXT("Shop.PriceSellEquipment"),
		OutResult, OutResults);

	return OutResult;
}

ACombatUnit_Summon* ULegendSystemLibrary::GenericSpawnSummon(const UObject* WorldContext, FName SummonID,
	AActor* SummonOwner, float InitCoefficient, float Lifetime)
{
	UFireflyGridBase* Grid = UFireflyGridMapLibrary::GetNearestVacantGridOfMap(WorldContext, SummonOwner);
	if (!Grid)
	{
		return nullptr;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return nullptr;
	}

	ULegendConfig_GameMode* ConfigGM = GetGameModeConfig(WorldContext);
	if (!ConfigGM)
	{
		return nullptr;
	}

	if (!ConfigGM->SummonBaseClass)
	{
		return nullptr;
	}

	ACombatUnit_Summon* OutSummon = World->SpawnActorDeferred<ACombatUnit_Summon>(ConfigGM->SummonBaseClass, Grid->WorldTransform,
		nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	UGameplayStatics::FinishSpawningActor(OutSummon, Grid->WorldTransform);

	OutSummon->InitializeCombatUnit(SummonID);
	OutSummon->InitOnRespawned(SummonOwner->GetInstigator(), Grid);
	OutSummon->InitOnSummoned(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SummonOwner), InitCoefficient, Lifetime);

	return OutSummon;
}