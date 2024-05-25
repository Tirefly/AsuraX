// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArenaGridMapBase.h"
#include "ArenaAuditorium.generated.h"


class UAllianceManagerComponent;


// 备战席
UCLASS()
class LEGENDSTD_API AArenaAuditorium : public AArenaGridMapBase
{
	GENERATED_BODY()

#pragma region Actor
	
public:	
	// Sets default values for this actor's properties
	AArenaAuditorium(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma endregion


#pragma region GridMap

public:
	virtual void GenerateGridMap() override;

public:
	// 阵营序号
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FireflyGridMap")
	FRotator SpawnRotation = FRotator::ZeroRotator;

#pragma endregion


#pragma region AllianceManager

public:
	// 获取阵营管理器
	UFUNCTION(BlueprintCallable, Category = Alliance)
	UAllianceManagerComponent* GetAllianceManager() const;

	// 获取阵营序号
	UFUNCTION(BlueprintCallable, Category = Alliance)
	int32 GetAllianceIndex() const { return AllianceIndex; }

protected:
	// 阵营序号
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FireflyGridMap")
	int32 AllianceIndex = -1;

#pragma endregion
};
