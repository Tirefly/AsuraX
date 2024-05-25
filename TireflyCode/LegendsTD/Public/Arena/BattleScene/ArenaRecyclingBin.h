// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaRecyclingBin.generated.h"


class UAllianceManagerComponent;


// 回收站
UCLASS()
class LEGENDSTD_API AArenaRecyclingBin : public AActor
{
	GENERATED_BODY()

#pragma region Actor

public:	
	// Sets default values for this actor's properties
	AArenaRecyclingBin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	UPROPERTY(EditAnywhere, Transient, Category = Alliance)
	int32 AllianceIndex = -1;

#pragma endregion
};
