// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SynergyUnitInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class USynergyUnitInterface : public UInterface
{
	GENERATED_BODY()
};


// 和羁绊系统相关的英雄的接口类，英雄应该是一个Pawn
class LEGENDSTD_API ISynergyUnitInterface
{
	GENERATED_BODY()
		
public:
	/** 获取单位的默认ID */
	UFUNCTION(BlueprintPure, Category = "CombatUnit|Synergy")
	virtual FName GetCombatUnitID() const { return NAME_None; }

	/** 获取单位的所有ID，包括默认的和从装备上获取的 */
	UFUNCTION(BlueprintPure, Category = "CombatUnit|Synergy")
	virtual TArray<FName> GetCombatUnitTotalIDs() const { return TArray<FName>{}; }

	/** 获取单位的所有相关羁绊的ID */
	UFUNCTION(BlueprintPure, Category = "CombatUnit|Synergy")
	virtual TArray<FName> GetCombatUnitSynergyIDs() const { return TArray<FName>{}; }
};
