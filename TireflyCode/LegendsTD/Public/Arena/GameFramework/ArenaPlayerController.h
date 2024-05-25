// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Synergy/SynergyControllerInterface.h"
#include "Arena/Alliance/AllianceControllerInterface.h"
#include "ArenaPlayerController.generated.h"


class UAllianceManagerComponent;
enum class ELegendGameStage : uint8;


// 玩家阵营的控制器
UCLASS()
class LEGENDSTD_API AArenaPlayerController : public APlayerController
	, public ISynergyControllerInterface
	, public IAllianceControllerInterface
{
	GENERATED_BODY()

#pragma region PlayerController

public:
	AArenaPlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

#pragma endregion


#pragma region SynergyControllerInterface

public:
	/** 获取Controller的羁绊管理器组件 */
	UFUNCTION()
	virtual USynergyManagerComponent* GetSynergyManager() const override;

	/** 获取Controller控制的角色（即玩家角色或敌人角色） */
	UFUNCTION(BlueprintCallable, Category = "Controller|Synergy")
	virtual APawn* GetSynergyDirector() const override;

	/** 处理新的战斗单位加入羁绊管理器的逻辑 */
	UFUNCTION(BlueprintCallable, Category = "Controller|Synergy")
	virtual void HandleCombatUnitJoined(AActor* InCombatUnit) override;

	/** 处理一个战斗单位离开羁绊管理器的逻辑 */
	UFUNCTION(BlueprintCallable, Category = "Controller|Synergy")
	virtual void HandleCombatUnitQuit(AActor* InCombatUnit) override;

	/** 处理一个由装备代表的战斗单位加入羁绊管理器的逻辑 */
	UFUNCTION(BlueprintCallable, Category = "Controller|Synergy")
	virtual void HandleEquipmentJoined(AActor* InCombatUnit, FName InEquipmentUnitID) override;

	/** 处理一个由装备代表的战斗单位离开羁绊管理器的逻辑 */
	UFUNCTION(BlueprintCallable, Category = "Controller|Synergy")
	virtual void HandleEquipmentQuit(APawn* InCombatUnit, FName InEquipmentUnitID) override;

protected:
	/** 玩家阵营控制器的羁绊管理器 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USynergyManagerComponent> SynergyManager;

#pragma endregion


#pragma region AllianceManager

public:
	/** 获取Controller的羁绊管理器组件 */
	UFUNCTION()
	virtual UAllianceManagerComponent* GetAllianceManager() const override { return AllianceManager; }

protected:
	// 玩家阵营控制器的阵营管理器
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAllianceManagerComponent> AllianceManager;

#pragma endregion
};
