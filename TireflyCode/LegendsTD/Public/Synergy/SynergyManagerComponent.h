// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreBasics/CoreGameplayTypes.h"
#include "SynergyManagerComponent.generated.h"


/** 游戏局内一个羁绊在待激活过程中的实时数据 */
USTRUCT(BlueprintType)
struct FSynergyRuntimePendingData
{
	GENERATED_BODY()

public:
	/** 羁绊的ID */
	UPROPERTY(BlueprintReadWrite)
	FName SynergyID;

	/** 羁绊的所有单位ID */
	UPROPERTY(BlueprintReadWrite)
	TArray<FName> FullUnitIDs;

	/** 羁绊尚未激活时已经收集的所有单位ID */
	UPROPERTY(BlueprintReadWrite)
	TArray<FName> PendingUnitIDs;

	/** 羁绊尚未激活时已经收集的所有单位ID */
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> SynergyCombatUnits;

	FSynergyRuntimePendingData() : SynergyID(NAME_None)
	{}

	FSynergyRuntimePendingData(FName InSynergyID, const TArray<FName>& InFullUnitIDs
		, const TArray<FName>& InPendingUnitIDs, const TArray<AActor*>& InCombatUnits)
		: SynergyID(InSynergyID), FullUnitIDs(InFullUnitIDs)
			, PendingUnitIDs(InPendingUnitIDs), SynergyCombatUnits(InCombatUnits)
	{}

	FORCEINLINE bool operator==(const FSynergyRuntimePendingData& Other) const
	{
		return SynergyID == Other.SynergyID
			&& FullUnitIDs == Other.FullUnitIDs
			&& PendingUnitIDs == Other.PendingUnitIDs
			&& SynergyCombatUnits == Other.SynergyCombatUnits;
	}

};


/** 游戏局内一个羁绊在激活后的实时数据 */
USTRUCT(BlueprintType)
struct FSynergyRuntimeActivatingData
{
	GENERATED_BODY()

public:
	/** 羁绊的ID */
	UPROPERTY(BlueprintReadWrite)
	FName SynergyID;

	/** 羁绊的所有单位ID */
	UPROPERTY(BlueprintReadWrite)
	TArray<FName> FullUnitIDs;

	/** 羁绊激活时，羁绊内的战斗单位以及对应的属性增强效果 */
	UPROPERTY(BlueprintReadWrite)
	TMap<AActor*, FActiveGameplayEffectHandle> SynergyCombatUnits;

	/** 该羁绊增强羁绊内角色属性的效果 */
	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectSpec SynergyAttributeEffect;

	FSynergyRuntimeActivatingData()
		: SynergyID(NAME_None), FullUnitIDs(TArray<FName>{})
	{}

	FSynergyRuntimeActivatingData(FName InSynergyID, const TArray<FName>& InFullUnitIDs, const TArray<AActor*>& InCombatUnits)
		: SynergyID(InSynergyID), FullUnitIDs(InFullUnitIDs)
	{
		for (AActor* CombatUnit : InCombatUnits)
		{
			SynergyCombatUnits.Add(CombatUnit);
		}
	}

	FORCEINLINE bool operator==(const FSynergyRuntimeActivatingData& Other) const
	{
		return SynergyID == Other.SynergyID
			&& FullUnitIDs == Other.FullUnitIDs;
	}
};


/**  游戏局内的羁绊管理器组件，应该被挂载到一个Controller上 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEGENDSTD_API USynergyManagerComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region ActorComponent

public:
	USynergyManagerComponent(const FObjectInitializer& ObjectInitializer);

#pragma endregion


#pragma region ProtectedFunctions

protected:
	/** 在羁绊管理器中激活特定的羁绊，仅应该被调用在Authority端 */
	void ActivateSynergy(const FName& InSynergyID, FSynergyRuntimeActivatingData& InSynergyData);

	/** 在羁绊管理器中使特定的羁绊失效，仅应该被调用在Authority端 */
	void InvalidateSynergy(const FName& InSynergyID, AActor* InCombatUnit);

	/** 处理战斗单位激活羁绊效果 */
	static void HandleCombatUnitActivateSynergy(AActor* InCombatUnit, FName SynergyID);

	/** 处理战斗单位取消激活羁绊效果 */
	static void HandleCombatUnitInvalidateSynergy(AActor* InCombatUnit, FName SynergyID);

#pragma endregion


#pragma region QueryFunctions

public:
	/** 判定一个羁绊在该羁绊管理器中是否已经被激活 */
	UFUNCTION(BlueprintPure, Category = SynergyManager)
	bool IsSynergyAlreadyActivated(const FName& InSynergyID) const;

	/** 判定一个羁绊在该羁绊管理器中是否尚未被激活 */
	UFUNCTION(BlueprintPure, Category = SynergyManager)
	bool IsSynergyPendingActivated(const FName& InSynergyID) const;

	/** 判定一个尚未被激活的羁绊中是否缺少某个单位 */
	UFUNCTION(BlueprintPure, Category = SynergyManager)
	bool DoesPendingSynergyLackUnit(const FName& InSynergyID, const FName& InCombatUnitID) const;

	/** 判定某个单位在当前阵营中是否还有其他相同的副本单位 */
	UFUNCTION(BlueprintPure, Category = SynergyManager)
	bool IsCombatUnitAlreadySpawned(const FName& InCombatUnitID);

#pragma endregion


#pragma region PublicFunctions

public:
	/** 当己方新单位被生成时，应该被Controller调用的函数，仅应该被调用在Authority端 */
	UFUNCTION(BlueprintCallable, Category = SynergyManager)
	void HandleCombatUnitJoined(AActor* InCombatUnit);

	/** 当己方单位将被销毁时，应该被Controller调用的函数，仅应该被调用在Authority端 */
	UFUNCTION(BlueprintCallable, Category = SynergyManager)
	void HandleCombatUnitQuit(AActor* InCombatUnit);

	/** 当己方单位的装备成功组成一个单位ID时，应该被Controller调用的函数，仅应该被调用在Authority端 */
	UFUNCTION(BlueprintCallable, Category = SynergyManager)
	void HandleEquipmentJoined(AActor* InCombatUnit, const FName& InEquipmentID);

	/** 当己方单位的装备无法组成一个单位ID时，应该被Controller调用的函数，仅应该被调用在Authority端 */
	UFUNCTION(BlueprintCallable, Category = SynergyManager)
	void HandleEquipmentQuit(AActor* InCombatUnit, const FName& InEquipmentID);

#pragma endregion


#pragma region Variables

protected:
	/** 该羁绊管理器下已激活的所有羁绊 */
	UPROPERTY(BlueprintReadWrite, Category = SynergyManager)
	TMap<FName, FSynergyRuntimeActivatingData> ActivatingSynergies;

	/** 该羁绊管理器下待激活的所有羁绊 */
	UPROPERTY(BlueprintReadWrite, Category = SynergyManager)
	TMap<FName, FSynergyRuntimePendingData> PendingSynergies;

#pragma endregion


#pragma region Delegates

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSynergyDelegate, const FName&, SynergyID, const TArray<FName>&, UnitIDs);

public:
	/** 当羁绊管理器收到有羁绊已激活时触发的代理 */
	UPROPERTY(BlueprintAssignable, Category = SynergyManager)
	FSynergyDelegate OnNewSynergyActivated;

	/** 当羁绊管理器收到有羁绊已失效时触发的代理 */
	UPROPERTY(BlueprintAssignable, Category = SynergyManager)
	FSynergyDelegate OnSynergyBeginPending;

	/** 当羁绊管理器收到有羁绊已失效时触发的代理 */
	UPROPERTY(BlueprintAssignable, Category = SynergyManager)
	FSynergyDelegate OnSynergyInvalidated;

	/** 当羁绊管理器的某个羁绊的相关单位发生改变时触发的代理 */
	UPROPERTY(BlueprintAssignable, Category = SynergyManager)
	FSynergyDelegate OnPendingSynergyUpdated;

#pragma endregion
};
