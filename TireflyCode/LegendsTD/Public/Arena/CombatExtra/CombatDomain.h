// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireflyPoolingActorInterface.h"
#include "CoreBasics/CoreGameplayTypes.h"
#include "CombatDomain.generated.h"


UCLASS()
class LEGENDSTD_API ACombatDomain : public AActor
	, public IFireflyPoolingActorInterface
{
	GENERATED_BODY()

#pragma region Actor
	
public:
	// Sets default values for this actor's properties
	ACombatDomain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma endregion


#pragma region FireflyPoolingActorInterface

public:
	virtual void PoolingBeginPlay_Implementation() override;

	virtual void PoolingEndPlay_Implementation() override;

	virtual FName PoolingGetActorID_Implementation() const override { return DomainID; }

	virtual void PoolingSetActorID_Implementation(FName NewActorID) override { DomainID = NewActorID; }

#pragma endregion


#pragma region Components

protected:
	// 领域的根组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombatExtra", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> DomainRoot;

	// 领域的静态模型组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombatExtra", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> DomainMesh;

	// 领域的NiagaraSystem组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombatExtra", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> DomainNS;

#pragma endregion


#pragma region DomainSettings

public:
	// 领域的ID
	UPROPERTY(Transient, BlueprintReadOnly, Category = "CombatExtra")
	FName DomainID = NAME_None;

	// 领域物的创造者
	UPROPERTY(Transient, BlueprintReadOnly, Category = "CombatExtra")
	TObjectPtr<AActor> DomainInstigator = nullptr;

	// 领域是否跟随创造者
	UPROPERTY(Transient, BlueprintReadOnly, Category = "CombatExtra")
	bool bFollowInstigator = false;

	// 领域的范围
	UPROPERTY(Transient, BlueprintReadOnly, Category = "CombatExtra")
	FVector DomainRange;

	// 领域的局内初始化
	UPROPERTY(Transient, BlueprintReadOnly, Category = "CombaatExtra")
	FCombatExtraSkin DomainSkin;

	// 领域的局内初始化
	UPROPERTY(Transient, BlueprintReadOnly, Category = "CombaatExtra")
	FCombatDomainEffect DomainEffect;

#pragma endregion


#pragma region DomainRuntime

public:
	UFUNCTION(BlueprintCallable, Category = "CombatExtra")
	void InitDomainOnRespawn(AActor* InInstigator, const FVector& Range); 

protected:
	// 领域执行逻辑的计时器
	UPROPERTY(Transient, BlueprintReadWrite, Category = "CombatExtra")
	FTimerHandle DomainExecutionTimer;

	// 游戏时在领域中的所有Actor
	UPROPERTY(Transient, BlueprintReadWrite, Category = "CombatExtra")
	TArray<AActor*> ActorsInDomain;

	// 游戏时在领域发起者的阵营
	UPROPERTY(Transient, BlueprintReadWrite, Category = "CombatExtra")
	TObjectPtr<AController> DomainInstigatorAlliance;

#pragma endregion


#pragma region DomainFunctions

protected:
	void InitDomainSkin() const;

public:
	/** 执行领域的检测 */
	UFUNCTION(BlueprintNativeEvent, Category = "CombatExtra")
	void ExecuteDomainCheck();
	virtual void ExecuteDomainCheck_Implementation();

	/** 执行领域的碰撞检测 */
	UFUNCTION(BlueprintNativeEvent, Category = "CombatExtra")
	bool TraceCheckDomainTargets(TArray<AActor*>& OutActors);
	virtual bool TraceCheckDomainTargets_Implementation(TArray<AActor*>& OutActors);

	/** 处理领域被销毁前的逻辑 */
	UFUNCTION(BlueprintNativeEvent, Category = "CombatExtra")
	void HandleDomainPendingDestroy();
	virtual void HandleDomainPendingDestroy_Implementation();

#pragma endregion


#pragma region DomainFunctions_Enemy

public:
	/** 当有敌人进入领域 */
	UFUNCTION(BlueprintNativeEvent, Category = "CombatExtra")
	void OnEnemyEnterDomain(AActor* NewEnemy);
	virtual void OnEnemyEnterDomain_Implementation(AActor* NewEnemy);

	/** 当有敌人离开领域 */
	UFUNCTION(BlueprintNativeEvent, Category = "CombatExtra")
	void OnEnemyExitedDomain(AActor* EnemyExited);
	virtual void OnEnemyExitedDomain_Implementation(AActor* EnemyExited);

	/** 当领域将被销毁时，对仍然在领域内的敌人处理的逻辑 */
	UFUNCTION(BlueprintNativeEvent, Category = "CombatExtra")
	void HandleEnemyInRangeOnDomainPendingDestroy(AActor* Enemy);
	virtual void HandleEnemyInRangeOnDomainPendingDestroy_Implementation(AActor* Enemy);

#pragma endregion


#pragma region DomainFunctions_Ally

public:
	/** 当有友方进入领域 */
	UFUNCTION(BlueprintNativeEvent, Category = "CombatExtra")
	void OnAllyEnterDomain(AActor* NewAlly);
	virtual void OnAllyEnterDomain_Implementation(AActor* NewAlly);

	/** 当有友方离开领域 */
	UFUNCTION(BlueprintNativeEvent, Category = "CombatExtra")
	void OnAllyExitedDomain(AActor* AllyExited);
	virtual void OnAllyExitedDomain_Implementation(AActor* AllyExited);

	/** 当领域将被销毁时，对仍然在领域内的友方处理的逻辑 */
	UFUNCTION(BlueprintNativeEvent, Category = "CombatExtra")
	void HandleAllyInRangeOnDomainPendingDestroy(AActor* Ally);
	virtual void HandleAllyInRangeOnDomainPendingDestroy_Implementation(AActor* Ally);

#pragma endregion
};
