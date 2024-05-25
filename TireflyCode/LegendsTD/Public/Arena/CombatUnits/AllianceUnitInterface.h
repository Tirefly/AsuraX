// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AllianceUnitInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UAllianceUnitInterface : public UInterface
{
	GENERATED_BODY()
};


// 阵营中的战斗单位要实现的接口类
class LEGENDSTD_API IAllianceUnitInterface
{
	GENERATED_BODY()

public:
	// 获取战斗单位的阵营控制器
	UFUNCTION(BlueprintPure, Category = CombatUnit)
	virtual AController* GetCurrentAlliance() const { return nullptr; }

	// 设置战斗单位的阵营控制器
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual void SetCurrentAlliance(AController* NewAlliance) {}

	// 获取战斗单位的原始阵营控制器
	UFUNCTION(BlueprintPure, Category = CombatUnit)
	virtual AController* GetOriginAlliance() const { return nullptr; }

	// 将战斗单位调整回原本的阵营
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual void BackToOriginAlliance() {}

	// 获取当前的敌对阵营目标
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual AActor* GetCurrentTarget() const { return nullptr; }
};
