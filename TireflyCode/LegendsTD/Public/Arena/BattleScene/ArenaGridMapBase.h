// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FireflyGridMapBase.h"
#include "ArenaGridMapBase.generated.h"


UCLASS(Config = Game)
class LEGENDSTD_API AArenaGridMapBase : public AFireflyGridMapBase
{
	GENERATED_BODY()

public:
	AArenaGridMapBase(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(Config, BlueprintReadOnly, Category = "FireflyGridMap")
	float GlobalGridRadius = 120.f;	
};
