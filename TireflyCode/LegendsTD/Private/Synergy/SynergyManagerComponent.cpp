// Fill out your copyright notice in the Description page of Project Settings.


#include "Synergy/SynergyManagerComponent.h"

#include "Synergy/SynergyUnitInterface.h"

#include "DataAsset/LegendSynergyDataAsset.h"
#include "CoreBasics/CoreLogChannels.h"
#include "Libraries/LegendSystemLibrary.h"


// Sets default values for this component's properties
USynergyManagerComponent::USynergyManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}

void USynergyManagerComponent::ActivateSynergy(const FName& InSynergyID
	, FSynergyRuntimeActivatingData& InSynergyData)
{
	// 获取羁绊的数据
	const ULegendSynergyDataAsset* SynergyDA = ULegendSystemLibrary::GetSynergyDataAsset(InSynergyID);
	if (!SynergyDA)
	{
		return;
	}

	// TODO: 获取羁绊激活时，羁绊内英雄三维属性的提升设置
	// TODO: 计算当前羁绊的英雄三维属性提升量
	// TODO: 生成羁绊属性提升效果的GameplayEffectSpec
	// TODO: 让羁绊所属阵营的玩家角色激活羁绊的技能系统数据

	// 让所有和该羁绊相关的英雄都激活对应的羁绊效果
	for (const auto SynergyUnitData : InSynergyData.SynergyCombatUnits)
	{
		const ISynergyUnitInterface* CurrentSynergyUnit = Cast<ISynergyUnitInterface>(SynergyUnitData.Key);
		if (!CurrentSynergyUnit)
		{
			continue;
		}
	
		const FName CurrentUnitID = CurrentSynergyUnit->GetCombatUnitID();
		if (!InSynergyData.FullUnitIDs.Contains(CurrentUnitID))
		{
			continue;
		}

		HandleCombatUnitActivateSynergy(SynergyUnitData.Key, InSynergyID);
	}

	// 处理完羁绊效果的激活，将该羁绊标记为激活状态
	ActivatingSynergies.Add(InSynergyID, InSynergyData);
	// 广播新羁绊激活的消息
	OnNewSynergyActivated.Broadcast(InSynergyID, InSynergyData.FullUnitIDs);
}

void USynergyManagerComponent::InvalidateSynergy(const FName& InSynergyID, AActor* InCombatUnit)
{
	if (!IsSynergyAlreadyActivated(InSynergyID))
	{
		return;
	}

	// 获取羁绊的数据
	const ULegendSynergyDataAsset* SynergyDA = ULegendSystemLibrary::GetSynergyDataAsset(InSynergyID);
	if (!IsValid(SynergyDA) || !IsValid(InCombatUnit))
	{
		return;
	}

	const ISynergyUnitInterface* SynergyUnit = Cast<ISynergyUnitInterface>(InCombatUnit);
	if (!SynergyUnit)
	{
		return;
	}

	// 收集待取消羁绊的数据
	const FSynergyRuntimeActivatingData SynergyToInvalidate = ActivatingSynergies.FindAndRemoveChecked(InSynergyID);
	TArray<AActor*> SynergyUnits;
	SynergyToInvalidate.SynergyCombatUnits.GenerateKeyArray(SynergyUnits);

	// 取消该羁绊的激活状态标记
	FSynergyRuntimePendingData NewRuntimePendingData;
	NewRuntimePendingData.SynergyID = SynergyToInvalidate.SynergyID;
	NewRuntimePendingData.FullUnitIDs = SynergyToInvalidate.FullUnitIDs;
	NewRuntimePendingData.PendingUnitIDs = SynergyToInvalidate.FullUnitIDs;
	NewRuntimePendingData.PendingUnitIDs.RemoveSingle(SynergyUnit->GetCombatUnitID());
	
	NewRuntimePendingData.SynergyCombatUnits = SynergyUnits;
	NewRuntimePendingData.SynergyCombatUnits.RemoveSingle(InCombatUnit);

	// 让阵营控制器控制的玩家角色取消激活羁绊效果
	// TODO: 让羁绊所属阵营的玩家角色取消激活羁绊的技能系统数据

	// 取消该羁绊所有相关英雄的该羁绊效果
	for (AActor* CurrentUnit : SynergyUnits)
	{
		const ISynergyUnitInterface* CurrentSynergyUnit = Cast<ISynergyUnitInterface>(CurrentUnit);
		if (!CurrentSynergyUnit)
		{
			continue;
		}
	
		const FName CurrentUnitID = CurrentSynergyUnit->GetCombatUnitID();
		if (!NewRuntimePendingData.PendingUnitIDs.Contains(CurrentUnitID))
		{
			continue;
		}

		HandleCombatUnitInvalidateSynergy(CurrentUnit, InSynergyID);
	}

	// 处理完羁绊效果的取消激活，将该羁绊标记为待激活状态
	if (NewRuntimePendingData.SynergyCombatUnits.Num() > 0)
	{
		PendingSynergies.Add(InSynergyID, NewRuntimePendingData);
	}
	// 广播有羁绊被取消的消息
	OnSynergyInvalidated.Broadcast(InSynergyID, NewRuntimePendingData.PendingUnitIDs);
}

void USynergyManagerComponent::HandleCombatUnitActivateSynergy(AActor* InCombatUnit, FName SynergyID)
{
	// TODO:为羁绊的战斗单位增加羁绊增强的三维属性
	// TODO:为战斗单位赋予羁绊相关的技能和效果
}

void USynergyManagerComponent::HandleCombatUnitInvalidateSynergy(AActor* InCombatUnit, FName SynergyID)
{
	// TODO:移除羁绊对战斗单位三维属性的增强
	// TODO:为战斗单位移除羁绊相关的技能和效果
}

bool USynergyManagerComponent::IsSynergyAlreadyActivated(const FName& InSynergyID) const
{
	return ActivatingSynergies.Find(InSynergyID) != nullptr;
}

bool USynergyManagerComponent::IsSynergyPendingActivated(const FName& InSynergyID) const
{
	return PendingSynergies.Find(InSynergyID) != nullptr;
}

bool USynergyManagerComponent::DoesPendingSynergyLackUnit(const FName& InSynergyID, const FName& InUnitID) const
{
	if (!IsSynergyPendingActivated(InSynergyID))
	{
		return false;
	}

	return !PendingSynergies.Find(InSynergyID)->PendingUnitIDs.Contains(InUnitID);
}

bool USynergyManagerComponent::IsCombatUnitAlreadySpawned(const FName& InCombatUnitID)
{
	int32 Count = 0;
	for (const auto Synergy : ActivatingSynergies)
	{
		TArray<AActor*> SynergyUnits;
		Synergy.Value.SynergyCombatUnits.GenerateKeyArray(SynergyUnits);
		for (const AActor* CombatUnit : SynergyUnits)
		{
			if (!IsValid(CombatUnit))
			{
				continue;
			}

			const ISynergyUnitInterface* SynergyUnit = Cast<ISynergyUnitInterface>(CombatUnit);
			if (!SynergyUnit)
			{
				continue;
			}

			if (SynergyUnit->GetCombatUnitID() == InCombatUnitID)
			{
				++Count;
				if (Count > 1)
				{
					return true;
				}
			}
		}
	}

	for (const auto Synergy : PendingSynergies)
	{
		for (const AActor* CombatUnit : Synergy.Value.SynergyCombatUnits)
		{
			if (!IsValid(CombatUnit))
			{
				continue;
			}

			const ISynergyUnitInterface* SynergyUnit = Cast<ISynergyUnitInterface>(CombatUnit);
			if (!SynergyUnit)
			{
				continue;
			}

			if (SynergyUnit->GetCombatUnitID() == InCombatUnitID)
			{
				++Count;
				if (Count > 1)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void USynergyManagerComponent::HandleCombatUnitJoined(AActor* InCombatUnit)
{
	if (!IsValid(InCombatUnit))
	{
		return;
	}

	const ISynergyUnitInterface* SynergyUnit = Cast<ISynergyUnitInterface>(InCombatUnit);
	if (!SynergyUnit)
	{
		return;
	}

	// 获取传入的羁绊单位的ID和与该单位关联的所有羁绊ID（UnitSynergyIDs）
	const FName UnitID = SynergyUnit->GetCombatUnitID();
	TArray<FName> UnitSynergyIDs = SynergyUnit->GetCombatUnitSynergyIDs();
	// 创建一个空数组存储该单位已经激活的羁绊
	TArray<FName> UnitActivatedSynergies;

	// 遍历该单位的所有羁绊
	for (auto SynergyID : UnitSynergyIDs)
	{
		// 尝试找到已经激活的羁绊
		if (FSynergyRuntimeActivatingData* SynergyData = ActivatingSynergies.Find(SynergyID))
		{
			// 若已经激活的羁绊包含该单位的技能系统信息，让该单位激活相对应的羁绊技能系统效果
			SynergyData->SynergyCombatUnits.Add(InCombatUnit);
			HandleCombatUnitActivateSynergy(InCombatUnit, SynergyID);
			UnitActivatedSynergies.Add(SynergyID);
		}
	}

	// 将已经激活的羁绊从待处理列表中剔除
	for (auto SynergyID : UnitActivatedSynergies)
	{
		UnitSynergyIDs.RemoveSingleSwap(SynergyID);
	}

	for (FName SynergyID : UnitSynergyIDs)
	{
		// 如果新生成的单位所属的羁绊尚未被激活
		if (FSynergyRuntimePendingData* Synergy = PendingSynergies.Find(SynergyID))
		{
			// 如果该单位已经被加入到对应羁绊的单位列表里，则快进到下一个羁绊
			if (Synergy->SynergyCombatUnits.Contains(InCombatUnit))
			{
				continue;
			}

			// 如果该单位在对应羁绊的ID列表中已存在，则将该单位加入到羁绊的单位列表后，快进到下一个羁绊
			if (Synergy->PendingUnitIDs.Contains(SynergyUnit->GetCombatUnitID()))
			{
				Synergy->SynergyCombatUnits.AddUnique(InCombatUnit);
				continue;
			}

			// 将新的战斗单位ID加入到待激活羁绊的单位列表中
			Synergy->SynergyCombatUnits.AddUnique(InCombatUnit);
			Synergy->PendingUnitIDs.Add(SynergyUnit->GetCombatUnitID());
			// 广播有待激活羁绊的战斗单位发生改变的消息
			OnPendingSynergyUpdated.Broadcast(SynergyID, Synergy->PendingUnitIDs);

			// 如果待激活羁绊的单位列表在添加了新单位后依旧未激活，则快进到下一个羁绊
			if (Synergy->PendingUnitIDs.Num() != Synergy->FullUnitIDs.Num())
			{
				continue;
			}

			// 如果待激活羁绊的单位列表在添加了新单位后，使羁绊被激活，执行激活羁绊的相关逻辑
			FSynergyRuntimeActivatingData NewActivatingData = FSynergyRuntimeActivatingData(SynergyID
				, Synergy->PendingUnitIDs, Synergy->SynergyCombatUnits);
			ActivateSynergy(SynergyID, NewActivatingData);

			// 将新激活的羁绊从待激活列表中移除
			PendingSynergies.Remove(SynergyID);

			// 快进到下一个羁绊的处理
			continue;
		}

		// 如果新生成的英雄所属的羁绊尚未被激活，且该羁绊尚未被标记为待激活，则创建一个待激活羁绊数据，加入到待激活羁绊列表里
		{
			const ULegendSynergyDataAsset* SynergyDA = ULegendSystemLibrary::GetSynergyDataAsset(SynergyID);
			if (!IsValid(SynergyDA))
			{
				break;
			}

			TArray<AActor*> SynergyUnits = { InCombatUnit };
			TArray<FName> SynergyUnitIDs = { SynergyUnit->GetCombatUnitID() };

			// 创建待激活羁绊数据，并添加到待激活羁绊的列表中
			FSynergyRuntimePendingData NewPendingData = FSynergyRuntimePendingData(SynergyID, SynergyDA->SynergyUnitIDs
				, SynergyUnitIDs, SynergyUnits);
			PendingSynergies.Add(SynergyID, NewPendingData);

			//广播有新待激活羁绊出现的消息
			OnSynergyBeginPending.Broadcast(SynergyID, SynergyUnitIDs);
		}
	}
}

void USynergyManagerComponent::HandleCombatUnitQuit(AActor* InCombatUnit)
{
	if (!IsValid(InCombatUnit))
	{
		return;
	}

	const ISynergyUnitInterface* SynergyUnit = Cast<ISynergyUnitInterface>(InCombatUnit);
	if (!SynergyUnit)
	{
		return;
	}

	// 有单位ID（来自单位或者装备）要被移除时，检测其对已激活羁绊和待激活羁绊的影响
	const bool bHasDuplicatedUnit = IsCombatUnitAlreadySpawned(SynergyUnit->GetCombatUnitID());
	for (FName SynergyID : SynergyUnit->GetCombatUnitSynergyIDs())
	{
		// 如果该羁绊处于激活状态
		if (FSynergyRuntimeActivatingData* Synergy = ActivatingSynergies.Find(SynergyID))
		{
			// 如果该单位在当前阵营中还有其他相同的副本单位，则删除该单位不会影响羁绊的状态
			if (bHasDuplicatedUnit)
			{
				Synergy->SynergyCombatUnits.Remove(InCombatUnit);
				continue;
			}

			// 取消要被销毁的战斗单位的该羁绊的激活状态
			InvalidateSynergy(SynergyID, InCombatUnit);
			continue;
		}

		// 如果将要被销毁的单位的某个羁绊处于待激活状态
		if (FSynergyRuntimePendingData* Synergy = PendingSynergies.Find(SynergyID))
		{
			// 如果该单位在当前阵营中还有其他相同的副本单位，则删除该单位不会影响羁绊的状态
			if (bHasDuplicatedUnit)
			{
				Synergy->SynergyCombatUnits.RemoveSingle(InCombatUnit);
				continue;
			}

			// 将该英雄的数据从对应的待激活羁绊数据中删除
			Synergy->PendingUnitIDs.RemoveSingle(SynergyUnit->GetCombatUnitID());
			Synergy->SynergyCombatUnits.RemoveSingle(InCombatUnit);

			// 检测对应待激活羁绊在移除英雄数据后是否进入完全不激活状态
			if (Synergy->PendingUnitIDs.Num() == 0 || Synergy->SynergyCombatUnits.Num() == 0)
			{
				PendingSynergies.Remove(SynergyID);
			}

			// 广播有待激活羁绊的战斗单位发生改变的消息
			OnPendingSynergyUpdated.Broadcast(SynergyID, Synergy->PendingUnitIDs);
		}
	}
}

void USynergyManagerComponent::HandleEquipmentJoined(AActor* InCombatUnit, const FName& InEquipmentUnitID)
{
	// TODO: 装备附加羁绊角色
}

void USynergyManagerComponent::HandleEquipmentQuit(AActor* InUnit, const FName& InEquipmentUnitID)
{
	// TODO: 装备解除羁绊角色
}
