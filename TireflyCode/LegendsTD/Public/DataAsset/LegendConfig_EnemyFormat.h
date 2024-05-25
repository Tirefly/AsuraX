// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CoreBasics/CoreGameplayTypes.h"
#include "LegendConfig_EnemyFormat.generated.h"


UCLASS()
class LEGENDSTD_API ULegendConfig_EnemyFormat : public UPrimaryDataAsset
{
	GENERATED_BODY()

#pragma region PrimaryDataAsset

public:
	ULegendConfig_EnemyFormat(const FObjectInitializer& ObjectInitializer);

#pragma endregion


#pragma region EnemyFormatNumber

public:
	// 小兵房的敌人波数设置
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FCombatRoomEnemyWave MinionRoomSetting;

	// 精英房的敌人波数设置
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FCombatRoomEnemyWave EliteRoomSetting;

	// 首领房的敌人波数设置
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FCombatRoomEnemyWave BossRoomSetting;

#pragma endregion


#pragma region EnemyFormatTable

public:
	// 构建战斗房的数据
	UFUNCTION(BlueprintCallable, Category = GameMode)
	void ConstructCombatRoomData(ESynergyClanType ClanType, const FRogueRoomRuntime_Combat& InData, FRogueRoomRuntime_Combat& OutData);

public:
	// 敌人阵型数据表（按派系区分）
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<ESynergyClanType, TObjectPtr<UDataTable>> EnemyFormatTableData;

#pragma endregion
};
