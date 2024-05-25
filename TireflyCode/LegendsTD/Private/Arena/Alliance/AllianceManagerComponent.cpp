// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "Arena/Alliance/AllianceManagerComponent.h"

#include "Arena/CombatUnits/CombatUnit_Hero.h"
#include "Arena/GameFramework/ArenaGameMode.h"
#include "Arena/BattleScene/ArenaAuditorium.h"
#include "Arena/BattleScene/ArenaBattlefield.h"

#include "DataAsset/LegendConfig_GameMode.h"
#include "DataAsset/LegendBuilder_CombatUnit.h"

#include "FireflyObjectPoolWorldSubsystem.h"
#include "Libraries/LegendSystemLibrary.h"


// Sets default values for this component's properties
UAllianceManagerComponent::UAllianceManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

// Called when the game starts
void UAllianceManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const AArenaGameMode* ArenaGM = ULegendSystemLibrary::GetArenaGameMode(this))
	{
		if (ULegendConfig_GameMode* ConfigGM = ArenaGM->GetGameModeConfig())
		{
			GameModeConfig = ConfigGM;
			PerformUpgrade();
			AcquireMoney(GameModeConfig->InitialMoney);
		}
	}
}

APawn* UAllianceManagerComponent::GetOwnerControlledPawn() const
{
	if (const AController* Controller = Cast<AController>(GetOwner()))
	{
		return Controller->GetPawn();
	}

	return nullptr;
}

void UAllianceManagerComponent::SetCurrentGrade(int32 NewGrade)
{
	CurrentGrade = NewGrade;
	OnPlayerGradeChanged.Broadcast(CurrentGrade);
}

void UAllianceManagerComponent::PerformUpgrade_Implementation()
{
	if (!CanPerformUpgrade())
	{
		return;
	}

	const int32 AdditivePopulation = GameModeConfig->PopulationPerGrade.Eval(GetCurrentGrade() + 1, FString("")) - GetCurrentMaxPopulation();

	ConsumeMoney(GameModeConfig->PlayerUpgradeCost.Eval(GetCurrentGrade() + 1, FString("")));
	SetCurrentGrade(GetCurrentGrade() + 1);
	IncreaseMaxPopulation(AdditivePopulation);
}

bool UAllianceManagerComponent::CanPerformUpgrade() const
{
	if (!IsValid(GameModeConfig))
	{
		return false;
	}

	if (GameModeConfig->PlayerMaxGrade <= GetCurrentGrade())
	{
		return false;
	}

	return CanConsumeMoney(GameModeConfig->PlayerUpgradeCost.Eval(GetCurrentGrade() + 1, FString()));
}

float UAllianceManagerComponent::GetCurrentUpgradeCost() const
{
	if (!IsValid(GameModeConfig))
	{
		return -1.f;
	}

	if (GameModeConfig->PlayerMaxGrade >= GetCurrentGrade())
	{
		return 0.f;
	}

	return GameModeConfig->PlayerUpgradeCost.Eval(GetCurrentGrade() + 1, FString());
}

int32 UAllianceManagerComponent::GetMaxGrade() const
{
	if (!IsValid(GameModeConfig))
	{
		return 15;
	}

	return GameModeConfig->PlayerMaxGrade;
}

void UAllianceManagerComponent::IncreaseMaxPopulation(int32 Addition)
{
	CurrentMaxPopulation += Addition;
	OnPlayerPopulationChanged.Broadcast(CurrentPopulation, CurrentMaxPopulation);
}

bool UAllianceManagerComponent::HandleHeroOccupyPopulation(ACombatUnit_Hero* InHero)
{
	if (!IsValid(InHero))
	{
		return false;
	}

	if (CurrentPopulation + InHero->GetHeroPopulation() > CurrentMaxPopulation)
	{
		return false;
	}

	CurrentPopulation += InHero->GetHeroPopulation();
	OnPlayerPopulationChanged.Broadcast(CurrentPopulation, CurrentMaxPopulation);
	return true;
}

void UAllianceManagerComponent::HandleHeroLeavePopulation(ACombatUnit_Hero* HeroLeaved)
{
	if (!IsValid(HeroLeaved))
	{
		return;
	}

	CurrentPopulation -= HeroLeaved->GetHeroPopulation();

	OnPlayerPopulationChanged.Broadcast(CurrentPopulation, CurrentMaxPopulation);
}

void UAllianceManagerComponent::AcquireMoney(int32 MoneyAcquired)
{
	CurrentMoney += MoneyAcquired;
	OnPlayerMoneyChanged.Broadcast(CurrentMoney);
}

void UAllianceManagerComponent::ConsumeMoney(int32 MoneyToConsume)
{
	if (!CanConsumeMoney(MoneyToConsume))
	{
		return;
	}

	CurrentMoney = FMath::Max<float>(0, CurrentMoney - MoneyToConsume);
	OnPlayerMoneyChanged.Broadcast(CurrentMoney);
}

void UAllianceManagerComponent::SetCurrentAuditorium(AArenaAuditorium* InAuditorium)
{
	Auditorium = InAuditorium;
}

void UAllianceManagerComponent::SetCurrentBattlefield(AArenaBattlefield* InBattlefield)
{
	Battlefield = InBattlefield;
}

void UAllianceManagerComponent::SetCurrentRecyclingBin(AArenaRecyclingBin* InRecyclingBin)
{
	RecyclingBin = InRecyclingBin;
}

AArenaRecyclingBin* UAllianceManagerComponent::GetCurrentRecyclingBin() const
{
	return RecyclingBin;
}

bool UAllianceManagerComponent::HandleTryBuyHero(const ULegendBuilder_CombatUnit* CombatUnitBuilder)
{
	if (!IsValid(CombatUnitBuilder) || !IsValid(GameModeConfig) || !IsValid(GetOwnerControlledPawn())
		|| !GetValid(Battlefield) || !GetValid(Auditorium))
	{
		return false;
	}

	// 获取对象池子系统
	const auto SubsystemOP = GetWorld()->GetSubsystem<UFireflyObjectPoolWorldSubsystem>();
	if (!IsValid(SubsystemOP))
	{
		return false;
	}

	// 获取英雄核心数据
	const auto HeroCoreData = CombatUnitBuilder->CoreBuilderData.GetPtr<FHeroBuilderData>();
	if (!HeroCoreData)
	{
		return false;
	}

	// 判定当前玩家的金币是否足够购买英雄
	if (!CanConsumeMoney((GameModeConfig->GetHeroPurchasePrice(HeroCoreData->Rarity))))
	{
		return false;
	}

	// 获取英雄类
	if (!IsValid(GameModeConfig->HeroBaseClass))
	{
		return false;
	}

	// 尝试升级场上已有的相同英雄
	{
		int32 SameHeroNumber = 0;
		for (ACombatUnit_Hero* OtherHero : Heroes)
		{
			if (OtherHero->GetCombatUnitID() != CombatUnitBuilder->GetPrimaryAssetId().PrimaryAssetName)
			{
				continue;
			}

			// 升级已有英雄
			if (OtherHero->CanPerformUpgrade())
			{
				OtherHero->PerformUpgradeOperation();
				// 扣钱
				ConsumeMoney(GameModeConfig->GetHeroPurchasePrice(HeroCoreData->Rarity));
				return true;
			}

			// 相同英雄数量监控
			++SameHeroNumber;
			if (SameHeroNumber >= GameModeConfig->SameHeroMaxNumOnBattlefield)
			{
				// TODO：发送相同英雄监控消息
				return false;
			}
		}
	}

	// 尝试放置英雄到备战席上
	for (const auto Grid : Auditorium->GridsOfMap)
	{
		if (!IsValid(Grid.Value))
		{
			continue;
		}

		if (!Grid.Value->IsGridVacant())
		{
			continue;
		}

		// 生成英雄
		const FTransform SpawnTransform = Grid.Value->WorldTransform;
		ACombatUnit_Hero* NewHero = SubsystemOP->ActorPool_SpawnActor<ACombatUnit_Hero>(GameModeConfig->HeroBaseClass
			, CombatUnitBuilder->GetPrimaryAssetId().PrimaryAssetName, SpawnTransform, -1.f
			, nullptr, GetOwnerControlledPawn());
		// 把英雄放到座位中
		NewHero->SetCurrentMovementGrid(Grid.Value);
		
		// 扣钱
		ConsumeMoney(GameModeConfig->GetHeroPurchasePrice(HeroCoreData->Rarity));
		Heroes.Emplace(NewHero);

		break;
	}

	return false;
}

bool UAllianceManagerComponent::HandleTrySellHero(ACombatUnit_Hero* Hero)
{
	if (!IsValid(GameModeConfig) || !IsValid(Hero))
	{
		return false;
	}

	if (Hero->GetCurrentAlliance() != GetOwner())
	{
		return false;
	}

	// 从阵营管理器中移除英雄
	Heroes.RemoveSwap(Hero);
	// 获得卖英雄的金币
	AcquireMoney(GameModeConfig->GetHeroSellingPrice(Hero->GetHeroRarity(), Hero->GetCurrentGrade()));
	// 处理英雄单位被卖掉的逻辑
	Hero->HandleIsSold();

	return true;
}

bool UAllianceManagerComponent::HandleHeroPlacedInGrid(ACombatUnit_Hero* Hero, UFireflyGridBase* NewGrid)
{
	if (!IsValid(Hero) || !IsValid(NewGrid))
	{
		return false;
	}

	UFireflyGridBase* CurrentGrid = Hero->GetCurrentMovementGrid();
	ACombatUnit_Hero* AnotherHero = NewGrid->ActorsInGrid.Num() > 0
		? Cast<ACombatUnit_Hero>(NewGrid->ActorsInGrid[0]) : nullptr;

	if (NewGrid == CurrentGrid)
	{
		Hero->SetTransformToCurrentGrid();
		return true;
	}

	// 在同一区域更换座位，不做人口判断
	if (CurrentGrid->GridMap == NewGrid->GridMap)
	{
		if (!IsValid(AnotherHero))
		{
			Hero->SetCurrentMovementGrid(NewGrid);
		}
		else
		{
			AnotherHero->SetCurrentMovementGrid(nullptr);
			Hero->SetCurrentMovementGrid(NewGrid);
			AnotherHero->SetCurrentMovementGrid(CurrentGrid);
		}

		return true;
	}

	// 换区域，但是不存在另一个英雄
	if (!IsValid(AnotherHero))
	{
		// 要进入战场席
		if (CurrentGrid->GridMap->IsA(AArenaAuditorium::StaticClass()))
		{
			// 人口不合法
			if (!HandleHeroOccupyPopulation(Hero))
			{
				// TODO: 弹出人口已满提示
				return false;
			}

			Hero->SetCurrentMovementGrid(NewGrid);
			return true;
		}

		HandleHeroLeavePopulation(Hero);
		Hero->SetCurrentMovementGrid(NewGrid);
		return true;
	}

	// 计算切换座位人口是否合法
	ACombatUnit_Hero* HeroLeaved = nullptr;
	ACombatUnit_Hero* HeroOccupied = nullptr;
	if (CurrentGrid->GridMap->IsA(AArenaAuditorium::StaticClass()))
	{
		HeroLeaved = Hero;
		HeroOccupied = AnotherHero;
	}
	else
	{
		HeroLeaved = AnotherHero;
		HeroOccupied = Hero;
	}
	
	// 若人口不合法
	if (CurrentPopulation - HeroLeaved->GetHeroPopulation() + HeroOccupied->GetHeroPopulation() > CurrentMaxPopulation)
	{
		// TODO: 弹出人口已满提示
		return false;
	}

	// 换座位
	HandleHeroLeavePopulation(HeroLeaved);
	HandleHeroOccupyPopulation(HeroOccupied);
	AnotherHero->SetCurrentMovementGrid(nullptr);
	Hero->SetCurrentMovementGrid(NewGrid);
	AnotherHero->SetCurrentMovementGrid(CurrentGrid);

	return true;
}
