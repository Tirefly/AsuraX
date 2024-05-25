// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SynergyControllerInterface.generated.h"


class USynergyManagerComponent;


// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class USynergyControllerInterface : public UInterface
{
	GENERATED_BODY()
};


// 羁绊管理器组件所挂载的Controller需要继承的接口
class LEGENDSTD_API ISynergyControllerInterface
{
	GENERATED_BODY()
		
public:
	/** 获取Controller的羁绊管理器组件 */
	UFUNCTION(BlueprintCallable, Category = "Controller|Synergy")
	virtual USynergyManagerComponent* GetSynergyManager() const { return nullptr; }

	/** 获取Controller控制的角色（即玩家角色或敌人角色） */
	UFUNCTION(BlueprintCallable, Category = "Controller|Synergy")
	virtual APawn* GetSynergyDirector() const { return nullptr; }

	/** 处理新的战斗单位加入羁绊管理器的逻辑 */
	UFUNCTION(BlueprintCallable, Category = "Controller|Synergy")
	virtual void HandleCombatUnitJoined(AActor* InCombatUnit) {}

	/** 处理一个战斗单位离开羁绊管理器的逻辑 */
	UFUNCTION(BlueprintCallable, Category = "Controller|Synergy")
	virtual void HandleCombatUnitQuit(AActor* InCombatUnit) {}

	/** 处理一个由装备代表的战斗单位加入羁绊管理器的逻辑 */
	UFUNCTION(BlueprintCallable, Category = "Controller|Synergy")
	virtual void HandleEquipmentJoined(AActor* InCombatUnit, FName InEquipmentUnitID) {}

	/** 处理一个由装备代表的战斗单位离开羁绊管理器的逻辑 */
	UFUNCTION(BlueprintCallable, Category = "Controller|Synergy")
	virtual void HandleEquipmentQuit(APawn* InCombatUnit, FName InEquipmentUnitID) {}
};
