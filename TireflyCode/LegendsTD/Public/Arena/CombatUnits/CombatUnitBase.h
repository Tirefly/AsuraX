// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpecHandle.h"
#include "CoreBasics/CoreGameplayTypes.h"
#include "AllianceUnitInterface.h"
#include "FireflyPoolingActorInterface.h"
#include "GameplayAbilities/LegendAbilitySystemComponent.h"
#include "Synergy/SynergyUnitInterface.h"
#include "CombatUnitBase.generated.h"


class UFireflyGridBase;
class ULegendBuilder_CombatUnit;
class ULegendAbilitySystemComponent;
class UFireflyGridMovementComponent;


// 战斗单位的基类
UCLASS()
class LEGENDSTD_API ACombatUnitBase : public APawn
	, public IAbilitySystemInterface
	, public IAllianceUnitInterface
	, public ISynergyUnitInterface
	, public IFireflyPoolingActorInterface
{
	GENERATED_BODY()

#pragma region Pawn

public:
	// Sets default values for this character's properties
	ACombatUnitBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

#pragma endregion


#pragma region CombatUnit

public:
	// 根据新的ID初始化战斗单位的ID、类型、构建器
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void InitializeCombatUnit(FName InCombatUnitID);

	// 获取战斗单位的类型
	UFUNCTION(BlueprintPure, Category = CombatUnit)
	ELegendCombatUnitType GetCombatUnitType() const { return CombatUnitType; }

	// 获取战斗单位的构建器资产
	UFUNCTION(BlueprintPure, Category = CombatUnit)
	ULegendBuilder_CombatUnit* GetCombatUnitBuilder() const;

	// 战斗单位是否可以被视为目标
	UFUNCTION(BlueprintPure, Category = CombatUnit)
	virtual bool CanBeTargeted(ACombatUnitBase* Opponent) const;

	// 战斗单位被生成时的初始化
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual void InitOnRespawned(APawn* InInstigator, UFireflyGridBase* Grid);

protected:
	// 战斗单位的ID
	UPROPERTY(EditAnywhere, Category = CombatUnit)
	FName CombatUnitID;

	// 战斗单位的类型
	UPROPERTY()
	ELegendCombatUnitType CombatUnitType;

	// 战斗单位的构建器资产
	UPROPERTY()
	TObjectPtr<ULegendBuilder_CombatUnit> CombatUnitBuilder = nullptr;

#pragma endregion


#pragma region CombatUnit_Navigation

public:
	// 获取战斗单位的棋格移动组件
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	UFireflyGridMovementComponent* GetGridMovement() const { return GridMovement; }
	
	// 获取战斗单位当前所属的棋格位置
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	UFireflyGridBase* GetCurrentMovementGrid() const;
	
	// 设置战斗单位当前所属的棋格位置
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual void SetCurrentMovementGrid(UFireflyGridBase* InGrid);

	// 设置战斗单位当前所属的棋格位置
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual void SetTransformToCurrentGrid();

protected:
	// 棋格导航移动组件
	UPROPERTY(VisibleAnywhere, Category = CombatUnit, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFireflyGridMovementComponent> GridMovement;

	UPROPERTY(Transient)
	UFireflyGridBase* DeathGrid = nullptr;

#pragma endregion


#pragma region CombatUnit_Grade

public:
	// 战斗单位是否能够执行升级操作
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual bool CanPerformUpgrade() const;

protected:
	// 处理战斗单位当前等级更改后的逻辑
	virtual void HandleCurrentGradeChanged(int32 NewGrade, int32 OldGrade);

public:
	// 战斗单位执行升级操作
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual void PerformUpgradeOperation();

	// 直接将战斗单位的等级设置到某一等级
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual void SetCurrentGrade(int32 NewGrade = 1);

	// 直接将战斗单位的等级设置到某一等级
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	int32 GetCurrentGrade() const { return CurrentGrade; }

protected:
	// 战斗单位的最大等级
	int32 MaxGrade = 5;

	// 战斗单位的当前等级
	int32 CurrentGrade = 0;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentGradeChangeDelegate, int32, NewGrade);
	// 战斗单位当前等级改变要触发的委托
	UPROPERTY(BlueprintAssignable, Category = CombatUnit)
	FCurrentGradeChangeDelegate OnCurrentGradeChanged;

#pragma endregion


#pragma region CombatUnit_Skin

protected:
	UFUNCTION(BlueprintCallable, Category = CombatUnit, Meta = (BlueprintProtected = true))
	void InitializeCombatUnitSkin() const;

protected:
	// 根组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CombatUnit, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RootScene;
	
	// 骨架组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CombatUnit, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	// 右手武器组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CombatUnit, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> WeaponRight;

	// 左手武器组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CombatUnit, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> WeaponLeft;

	// 头部冠饰组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CombatUnit, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Headdress;

#pragma endregion


#pragma region CombatUnit_Alliance

public:
	// 获取战斗单位的阵营控制器
	UFUNCTION()
	virtual AController* GetCurrentAlliance() const override { return CurrentAlliance; }

	// 设置战斗单位的阵营控制器
	UFUNCTION()
	virtual void SetCurrentAlliance(AController* NewAlliance) override;

	// 获取战斗单位的原始阵营控制器
	UFUNCTION()
	virtual AController* GetOriginAlliance() const override;

	// 将战斗单位调整回原本的阵营
	UFUNCTION()
	virtual void BackToOriginAlliance() override;

	// 获取当前的敌对阵营目标
	UFUNCTION()
	virtual AActor* GetCurrentTarget() const override;

protected:
	// 战斗单位的当前阵营控制器
	UPROPERTY()
	AController* CurrentAlliance = nullptr;

#pragma endregion


#pragma region GameplayAbilities

protected:
	// 初始化战斗单位的能力系统
	virtual void InitializeAbilitySystem();

	// 初始化战斗单位的能力系统：自定义执行
	virtual void InitGAS_CustomExecution();

	// 初始化战斗单位的能力系统：游戏性属性
	virtual void InitGAS_GameplayAttribute();

	// 初始化战斗单位的能力系统：游戏性能力
	virtual void InitGAS_GameplayAbility();

	// 初始化战斗单位的能力系统：游戏性效果
	virtual void InitGAS_GameplayEffect();	

	// 处理战斗单位当前等级更改后的逻辑：属性
	virtual void HandleCurrentGradeChanged_GameplayAttribute(int32 AdaptedNewGrade, int32 OldGrade);

	// 处理战斗单位当前等级更改后的逻辑：能力
	virtual void HandleCurrentGradeChanged_GameplayAbility(int32 AdaptedNewGrade, int32 OldGrade);

	// 根据数据表初始化战斗单位
	void InitCombatUnitAttributes(const UDataTable* DataTable, FName RowName) const;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 获取战斗单位的主属性类型
	UFUNCTION(BlueprintCallable, Category = AbilitySystem)
	EMainAttributeType GetMainAttributeType() const;

	// 获取战斗单位的普通攻击能力
	UFUNCTION(BlueprintCallable, Category = AbilitySystem)
	const FGameplayAbilitySpecHandle& GetNormalAttackAbility() const { return NormalAttackAbility; }

	// 获取战斗单位的技能能力数组
	UFUNCTION(BlueprintCallable, Category = AbilitySystem)
	const TArray<FGameplayAbilitySpecHandle>& GetSkillAbilities() const { return SkillAbilities; }

	// 获取战斗单位的技能能力学习状态
	UFUNCTION(BlueprintCallable, Category = AbilitySystem)
	const TMap<ULegendGameplayAbilityAsset*, bool>& GetSkillsLearningState() const { return SkillsLearningState; }

protected:
	// 技能系统组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULegendAbilitySystemComponent> AbilitySystem;

	// 战斗单位的普通攻击能力
	UPROPERTY(Transient)
	FGameplayAbilitySpecHandle NormalAttackAbility;

	// 战斗单位的技能能力数组
	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> SkillAbilities;

	// 战斗单位的技能能力学习状态
	UPROPERTY(Transient)
	TMap<ULegendGameplayAbilityAsset*, bool> SkillsLearningState;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkillLearningStateDelegate
		, const ULegendGameplayAbilityAsset*, AbilitiesWithGrade, bool, bLearnt);
	// 战斗单位当前等级改变时，技能学习变更要触发的委托，用于UI
	UPROPERTY(BlueprintAssignable, Category = CombatUnit)
	FSkillLearningStateDelegate OnSkillLearningStateChanged;

#pragma endregion


#pragma region GameMode_GameStage

public:
	// 切换战斗单位大脑逻辑的开启和关闭
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void ToggleBrainLogic(bool bOpen);

	// 切换战斗单位大脑逻辑的开启和关闭
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void ToggleCollision(bool bEnable);

	// 切换显示状态
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void ToggleVisibility(bool bVisible);

	// 切换存活/死亡状态
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void ToggleAliveState(bool bAlive);

	// 切换原始位置/死亡位置
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void ToggleAliveLocation(bool bAlive);

	// 切换战斗单位加入/退出羁绊阵营
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void ToggleSynergy(bool bJoinSynergy);

	// 切换普通攻击能力的赋予/移除
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void ToggleNormalAttackAbility(bool bLearn);

	// 切换技能能力学习状态的统计数组
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void ToggleSkillAbilitiesLearningState(bool bInit);

	// 切换是否响应Pawn的碰撞响应
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void ToggleCollideWithPawn(ECollisionResponse NewResponse) const;

	// 重置战斗单位的属性值
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void ResetGameplayAttributeValues() const;

	// 移除所有战斗单位的能力
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void RemoveSkillAbilities();

	// 移除所有活跃的战斗效果
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void RemoveActiveCombatEffects() const;

	// 处理战斗单位将要被销毁的逻辑
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void HandleRecycled();

protected:
	UFUNCTION()
	virtual void HandleGameStageUpdated(ELegendGameStage NewGameStage, ELegendGameStage OldGameStage);

public:
	// 处理战场开战的逻辑
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual void HandleStartedBattle() {}

	// 处理战场结束战斗的逻辑
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual void HandleFinishedBattle() {}

	// 战斗单位开始死亡
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual void StartDead();

	// 战斗单位结束死亡
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual void FinishDead();

	// 战斗单位开始复活
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual void StartRevive();

	// 战斗单位结束复活
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual void FinishRevive();

	// 按最大生命值的指定百分比回复当前生命值
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void RecoverHealthByPercent(float Percent);

	// 按最大魔法值的指定百分比回复当前魔法值
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void RecoverManaByPercent(float Percent);

#pragma endregion


#pragma region FireflyObjectPool

public:
	// Actor从对象池中生成后执行的BeginPlay。
	virtual void PoolingBeginPlay_Implementation() override;

	// Actor被放回对象池中后执行的EndPlay。
	virtual void PoolingEndPlay_Implementation() override;

	// 获取Actor的ID。
	virtual FName PoolingGetActorID_Implementation() const override { return CombatUnitID; }

	// 设置Actor的ID。
	virtual void PoolingSetActorID_Implementation(FName NewActorID) override { CombatUnitID = NewActorID; }

#pragma endregion


#pragma region SynergySystem

public:
	/** 获取单位的默认ID */
	UFUNCTION()
	virtual FName GetCombatUnitID() const override { return CombatUnitID; }

	/** 获取单位的所有ID，包括默认的和从装备上获取的 */
	UFUNCTION()
	virtual TArray<FName> GetCombatUnitTotalIDs() const override { return TArray<FName>{}; }

	/** 获取单位的所有相关羁绊的ID */
	UFUNCTION()
	virtual TArray<FName> GetCombatUnitSynergyIDs() const override;

#pragma endregion
};
