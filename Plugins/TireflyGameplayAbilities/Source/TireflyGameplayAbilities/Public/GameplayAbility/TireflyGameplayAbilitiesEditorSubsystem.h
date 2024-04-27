// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TireflyGameplayAbilitiesEditorSubsystem.generated.h"

/**
 * 用于加载能力资产的引擎子系统
 */
UCLASS()
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbilitiesEditorSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

#pragma region Subsystem

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

#pragma endregion
};
