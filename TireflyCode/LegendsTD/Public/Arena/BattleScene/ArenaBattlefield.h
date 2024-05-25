// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArenaGridMapBase.h"
#include "ArenaBattlefield.generated.h"


class UAllianceManagerComponent;


// 战场
UCLASS()
class LEGENDSTD_API AArenaBattlefield : public AArenaGridMapBase
{
	GENERATED_BODY()

#pragma region Actor

public:	
	// Sets default values for this actor's properties
	AArenaBattlefield(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma endregion


#pragma region AllianceManager

public:
	// 获取阵营管理器
	UFUNCTION(BlueprintCallable, Category = "FireflyGridMap|Alliance")
	UAllianceManagerComponent* GetAllianceManager() const;

	// 获取阵营序号
	UFUNCTION(BlueprintCallable, Category = "FireflyGridMap|Alliance")
	int32 GetAllianceIndex() const { return AllianceIndex; }

protected:
	// 阵营序号
	UPROPERTY(EditAnywhere, Transient, Category = "FireflyGridMap|Alliance")
	int32 AllianceIndex = -1;

#pragma endregion


#pragma region GridMap

public:
	virtual void GenerateGridMap() override;

	UFUNCTION(BlueprintCallable, Category = "FireflyGridMap|Battlefield")
	bool IsGridInPlayerFormation(const FFireflyGridCoordinate& Grid) const;

	UFUNCTION(BlueprintCallable, Category = "FireflyGridMap|Battlefield")
	bool IsGridInBattlefield(const FFireflyGridCoordinate& Grid) const;

	UFUNCTION(BlueprintCallable, Category = "FireflyGridMap|Battlefield")
	bool IsGridInEnemyFormation(const FFireflyGridCoordinate& Grid) const;

protected:
	UPROPERTY(Transient)
	TArray<FFireflyGridCoordinate> PlayerFormationGrids;

	UPROPERTY(Transient)
	TArray<FFireflyGridCoordinate> BattlefieldGrids;

	UPROPERTY(Transient)
	TArray<FFireflyGridCoordinate> EnemyFormationGrids;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FireflyGridMap|Battlefield")
	int32 PlayerSideRow = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FireflyGridMap|Battlefield")
	FRotator PlayerSideRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FireflyGridMap|Battlefield")
	int32 EnemySideRow = 9;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FireflyGridMap|Battlefield")
	FRotator EnemySideRotation = FRotator(0.f, 180.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FireflyGridMap|Battlefield")
	int32 BattlefieldRow = 1;

#pragma endregion
};
