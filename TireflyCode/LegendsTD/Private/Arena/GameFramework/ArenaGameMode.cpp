// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/GameFramework/ArenaGameMode.h"

#include "Arena/Alliance/AllianceManagerComponent.h"
#include "Arena/BattleScene/ArenaRecyclingBin.h"
#include "Arena/BattleScene/ArenaBattlefield.h"
#include "Arena/BattleScene/ArenaAuditorium.h"
#include "Arena/GameFramework/ArenaEnemyController.h"
#include "Arena/GameFramework/ArenaPlayerController.h"

#include "CoreBasics/CoreGameplayTag.h"
#include "FireflyAbilitySystemLibrary.h"
#include "Arena/CombatUnits/CombatUnitBase.h"
#include "Kismet/GameplayStatics.h"


AArenaGameMode::AArenaGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	EnemyControllerClass = AArenaEnemyController::StaticClass();
}

void AArenaGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AArenaGameMode::StartPlay()
{
	Super::StartPlay();

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save player controllers into a map
	SpawnInfo.bDeferConstruction = true;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FVector SpawnLoc = FVector::ZeroVector;
	const FRotator SpawnRot = FRotator::ZeroRotator;

	for (int i = 0; i < EnemyAllianceNum; ++i)
	{
		if (AArenaEnemyController* NewEC = GetWorld()->SpawnActor<AArenaEnemyController>(EnemyControllerClass, SpawnLoc, SpawnRot, SpawnInfo))
		{
			UGameplayStatics::FinishSpawningActor(NewEC, FTransform(SpawnRot, SpawnLoc));
			if (APawn* NewEP = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLoc, SpawnRot, SpawnInfo))
			{
				UGameplayStatics::FinishSpawningActor(NewEP, FTransform(SpawnRot, SpawnLoc));
				NewEC->SetPawn(NewEP);
				NewEC->Possess(NewEC->GetPawn());

				EnemyAlliances.Add(NewEC);
			}
		}
	}
}

void AArenaGameMode::SetCurrentGameStage(ELegendGameStage InGameStage)
{
	const ELegendGameStage OldGameStage = CurrentStage;
	PreGameStageUpdated(InGameStage, OldGameStage);
	CurrentStage = InGameStage;
	PostGameStageUpdated(CurrentStage, OldGameStage);
	OnGameStageUpdated.Broadcast(CurrentStage, OldGameStage);
}

void AArenaGameMode::HandleCombatUnitJoinAlliance(AActor* CombatUnit, AController* NewAlliance, AController* OldAlliance)
{
	// 如果CombatUnit无效，NewAlliance无效，或者NewAlliance等于OldAlliance，则返回
	if (!IsValid(CombatUnit) || !IsValid(NewAlliance) || NewAlliance == OldAlliance)
	{
		return;
	}

	// 判断CombatUnit是否已经死亡
	const bool bIsCombatUnitDead = UFireflyAbilitySystemLibrary::ActorHasMatchingGameplayTag(
		CombatUnit, GAS_COMBATUNITSTATE_DEATH);
	// 标记是否存在其他联盟
	bool bNoExistingAlliance = true;

	// 遍历所有联盟的单位容器
	for (auto& UnitContainer : AllianceUnitContainers)
	{
		// 如果单位容器的键值等于OldAlliance
		if (UnitContainer.Key == OldAlliance)
		{
			// 如果CombatUnit已经死亡，则从联盟死亡单位中移除
			// 否则，从联盟存活单位中移除
			if (bIsCombatUnitDead)
			{
				UnitContainer.Value.AllianceDeadUnits.RemoveSingle(CombatUnit);
			}
			else
			{
				UnitContainer.Value.AllianceAliveUnits.RemoveSingle(CombatUnit);
			}

			continue;
		}

		// 如果单位容器的键值等于NewAlliance
		if (UnitContainer.Key == NewAlliance)
		{
			// 如果CombatUnit已经死亡，则添加到联盟死亡单位中
			// 否则，添加到联盟存活单位中
			if (bIsCombatUnitDead)
			{
				UnitContainer.Value.AllianceDeadUnits.AddUnique(CombatUnit);
			}
			else
			{
				UnitContainer.Value.AllianceAliveUnits.AddUnique(CombatUnit);
			}

			// 标记存在其他联盟
			bNoExistingAlliance = false;
		}
	}

	// 如果不存在其他联盟
	if (bNoExistingAlliance)
	{
		// 添加NewAlliance至联盟单位容器中
		FArenaAllianceUnitContainer& NewContainer = AllianceUnitContainers.Add(NewAlliance);
		// 如果CombatUnit已经死亡，则添加到联盟死亡单位中
		// 否则，添加到联盟存活单位中
		if (bIsCombatUnitDead)
		{
			NewContainer.AllianceDeadUnits.Emplace(CombatUnit);
		}
		else
		{
			NewContainer.AllianceAliveUnits.Emplace(CombatUnit);
		}
	}

	// 广播CombatUnit进入NewAlliance事件
	OnCombatUnitEnterAlliance.Broadcast(NewAlliance, CombatUnit);
}

void AArenaGameMode::HandleCombatUnitQuitAlliance(AActor* CombatUnit, AController* Alliance)
{
	if (!IsValid(CombatUnit) || !IsValid(Alliance))
	{
		return;
	}

	const bool bIsCombatUnitDead = UFireflyAbilitySystemLibrary::ActorHasMatchingGameplayTag(
		CombatUnit, GAS_COMBATUNITSTATE_DEATH);
	for (auto& UnitContainer : AllianceUnitContainers)
	{
		if (UnitContainer.Key == Alliance)
		{
			if (bIsCombatUnitDead)
			{
				UnitContainer.Value.AllianceDeadUnits.RemoveSingle(CombatUnit);
			}
			else
			{
				UnitContainer.Value.AllianceAliveUnits.RemoveSingle(CombatUnit);
			}

			OnCombatUnitQuitAlliance.Broadcast(Alliance, CombatUnit);
			break;
		}
	}
}

bool AArenaGameMode::IsCombatUnitInAlliance(AActor* CombatUnit, AController* Alliance) const
{
	const FArenaAllianceUnitContainer* UnitContainer = AllianceUnitContainers.Find(Alliance);
	if (!UnitContainer)
	{
		return false;
	}

	return UnitContainer->AllianceAliveUnits.Contains(CombatUnit)
		|| UnitContainer->AllianceDeadUnits.Contains(CombatUnit);
}

void AArenaGameMode::HandleCombatUnitReborn(AController* Alliance, AActor* CombatUnit)
{
	if (!HasAuthority())
	{
		return;
	}

	if (!IsValid(CombatUnit) || !IsValid(Alliance))
	{
		return;
	}

	if (FArenaAllianceUnitContainer* Container = AllianceUnitContainers.Find(Alliance))
	{
		Container->AllianceAliveUnits.AddUnique(CombatUnit);
		Container->AllianceDeadUnits.RemoveSingle(CombatUnit);

		ReceiveHandleCombatUnitReborn(Alliance, CombatUnit);
	}
}

void AArenaGameMode::HandleCombatUnitDead(AController* Alliance, AActor* CombatUnit)
{
	if (!HasAuthority())
	{
		return;
	}

	if (!IsValid(CombatUnit) || !IsValid(Alliance))
	{
		return;
	}

	if (FArenaAllianceUnitContainer* Container = AllianceUnitContainers.Find(Alliance))
	{
		Container->AllianceDeadUnits.AddUnique(CombatUnit);
		Container->AllianceAliveUnits.RemoveSingle(CombatUnit);
	}

	OnCombatUnitDead.Broadcast(Alliance, CombatUnit);

	ReceiveHandleCombatUnitDead(Alliance, CombatUnit);
}

void AArenaGameMode::HandlePlayerAllianceInitialized_Implementation(AArenaPlayerController* Player)
{
	PlayerAlliances.Add(Player);
	int32 AllianceIndex = -1;

	// 玩家阵营的战场
	TArray<AActor*> Battlefields;
	UGameplayStatics::GetAllActorsOfClass(this, AArenaBattlefield::StaticClass(), Battlefields);
	for (AActor* Actor : Battlefields)
	{
		if (AArenaBattlefield* Battlefield = Cast<AArenaBattlefield>(Actor))
		{
			if (AController* Controller = Cast<AController>(Battlefield->GetOwner()))
			{
				continue;
			}

			AllianceIndex = Battlefield->GetAllianceIndex();
			Battlefield->SetOwner(Player);
			Player->GetAllianceManager()->SetCurrentBattlefield(Battlefield);

			break;
		}
	}

	// 玩家阵营的备战席
	TArray<AActor*> Auditoriums;
	UGameplayStatics::GetAllActorsOfClass(this, AArenaAuditorium::StaticClass(), Auditoriums);
	for (AActor* Actor : Auditoriums)
	{
		if (AArenaAuditorium* Auditorium = Cast<AArenaAuditorium>(Actor))
		{
			if (AllianceIndex == Auditorium->GetAllianceIndex())
			{
				Auditorium->SetOwner(Player);
				Player->GetAllianceManager()->SetCurrentAuditorium(Auditorium);

				break;
			}
		}
	}

	// 玩家阵营的英雄回收站
	TArray<AActor*> RecyclingBins;
	UGameplayStatics::GetAllActorsOfClass(this, AArenaRecyclingBin::StaticClass(), RecyclingBins);
	for (AActor* Actor : RecyclingBins)
	{
		if (AArenaRecyclingBin* RecyclingBin = Cast<AArenaRecyclingBin>(Actor))
		{
			if (AllianceIndex == RecyclingBin->GetAllianceIndex())
			{
				RecyclingBin->SetOwner(Player);
				Player->GetAllianceManager()->SetCurrentRecyclingBin(RecyclingBin);

				break;
			}
		}
	}
}

void AArenaGameMode::HandleEnemyAllianceInitialized_Implementation(AArenaEnemyController* Enemy)
{
	EnemyAlliances.Add(Enemy);
	int32 AllianceIndex = -1;

	// 敌人阵营的战场
	TArray<AActor*> Battlefields;
	UGameplayStatics::GetAllActorsOfClass(this, AArenaBattlefield::StaticClass(), Battlefields);
	for (AActor* Actor : Battlefields)
	{
		if (AArenaBattlefield* Battlefield = Cast<AArenaBattlefield>(Actor))
		{
			if (AController* Controller = Cast<AController>(Battlefield->GetOwner()))
			{
				continue;
			}

			AllianceIndex = Battlefield->GetAllianceIndex();
			Battlefield->SetOwner(Enemy);

			break;
		}
	}

	// 敌人阵营的备战席
	TArray<AActor*> Auditoriums;
	UGameplayStatics::GetAllActorsOfClass(this, AArenaAuditorium::StaticClass(), Auditoriums);
	for (AActor* Actor : Auditoriums)
	{
		if (AArenaAuditorium* Auditorium = Cast<AArenaAuditorium>(Actor))
		{
			if (AController* Controller = Cast<AController>(Auditorium->GetOwner()))
			{
				continue;
			}

			if (AllianceIndex != Auditorium->GetAllianceIndex())
			{
				continue;
			}

			Auditorium->SetOwner(Enemy);
			break;
		}
	}

	// 敌人阵营的英雄回收站
	TArray<AActor*> RecyclingBins;
	UGameplayStatics::GetAllActorsOfClass(this, AArenaRecyclingBin::StaticClass(), RecyclingBins);
	for (AActor* Actor : RecyclingBins)
	{
		if (AArenaRecyclingBin* RecyclingBin = Cast<AArenaRecyclingBin>(Actor))
		{
			if (AllianceIndex == RecyclingBin->GetAllianceIndex())
			{
				RecyclingBin->SetOwner(Enemy);
				Enemy->GetAllianceManager()->SetCurrentRecyclingBin(RecyclingBin);

				break;
			}
		}
	}
}
