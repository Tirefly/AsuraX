// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "DataAsset/LegendConfig_EnemyFormat.h"

#include "Kismet/DataTableFunctionLibrary.h"


ULegendConfig_EnemyFormat::ULegendConfig_EnemyFormat(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULegendConfig_EnemyFormat::ConstructCombatRoomData(ESynergyClanType ClanType,
	const FRogueRoomRuntime_Combat& InData, FRogueRoomRuntime_Combat& OutData)
{
	OutData = InData;
	if (OutData.RoomType == ERogueRoomType::Event || OutData.RoomType == ERogueRoomType::Shop
		|| OutData.RoomType == ERogueRoomType::None)
	{
		return;
	}

	UDataTable* FormatTable = EnemyFormatTableData.FindRef(ClanType);
	if (!FormatTable)
	{
		return;
	}

	TArray<FName> MinionFormats;
	TArray<FName> EliteFormats;
	TArray<FName> BossFormats;

	TArray<FName> Formats;
	UDataTableFunctionLibrary::GetDataTableRowNames(FormatTable, Formats);
	for (const FName Format : Formats)
	{
		const FString FormatStr = Format.ToString();
		if (!FormatStr.IsValidIndex(4))
		{
			continue;
		}

		if (FormatStr[4] == '1')
		{
			MinionFormats.AddUnique(Format);
			continue;
		}
		if (FormatStr[4] == '2')
		{
			EliteFormats.AddUnique(Format);
			continue;
		}
		if (FormatStr[4] == '3')
		{
			BossFormats.AddUnique(Format);
		}
	}

	FCombatRoomEnemyWave TmpWaveSetting;
	switch (OutData.RoomType)
	{
		case ERogueRoomType::Minion:
		{
			TmpWaveSetting = MinionRoomSetting;
			break;
		}
		case ERogueRoomType::Elite:
		{
			TmpWaveSetting = EliteRoomSetting;
			break;
		}
		case ERogueRoomType::Boss:
		{
			TmpWaveSetting = BossRoomSetting;
			break;
		}
		default:
			return;
	}

	int32 TmpFormatIdx = -1;

	// 填充小兵攻击阵型
	for (int32 i = 0; i < TmpWaveSetting.MinionWaveNum; ++i)
	{
		TmpFormatIdx = FMath::RandRange(0, MinionFormats.Num() - 1);
		OutData.EnemyFormats.Emplace(MinionFormats[TmpFormatIdx]);

		// TmpMinionWaves.RemoveAt(TmpFormatIdx);
	}

	// 填充精英攻击阵型
	for (int32 i = 0; i < TmpWaveSetting.EliteWaveNum; ++i)
	{
		TmpFormatIdx = FMath::RandRange(0, EliteFormats.Num() - 1);
		OutData.EnemyFormats.Emplace(EliteFormats[TmpFormatIdx]);

		EliteFormats.RemoveAt(TmpFormatIdx);
	}

	// 填充首领攻击阵型
	for (int32 i = 0; i < TmpWaveSetting.BossWaveNum; ++i)
	{
		TmpFormatIdx = FMath::RandRange(0, BossFormats.Num() - 1);
		OutData.EnemyFormats.Emplace(BossFormats[TmpFormatIdx]);

		BossFormats.RemoveAt(TmpFormatIdx);
	}
}
