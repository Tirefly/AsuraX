// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "FireflyAbilitySystemEngineSubsystem.generated.h"


// 技能系统的引擎子系统
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyAbilitySystemEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
