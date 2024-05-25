// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireflyPoolingActorInterface.h"
#include "GameplayEffectTypes.h"
#include "CoreBasics/CoreGameplayTypes.h"
#include "CombatProjectile.generated.h"


class UAbilitySystemComponent;


UCLASS()
class LEGENDSTD_API ACombatProjectile : public AActor
	, public IFireflyPoolingActorInterface
{
	GENERATED_BODY()

#pragma region Actor

public:
	// Sets default values for this actor's properties
	ACombatProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma endregion


#pragma region FireflyPoolingActorInterface

public:
	virtual void PoolingBeginPlay_Implementation() override;

	virtual void PoolingEndPlay_Implementation() override;

	virtual FName PoolingGetActorID_Implementation() const override { return ProjectileID; }

	virtual void PoolingSetActorID_Implementation(FName NewActorID) override { ProjectileID = NewActorID; }

#pragma endregion


#pragma region Components

protected:
	// 抛射物的根组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombaatExtra", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> ProjectileRoot;

	// 抛射物的静态模型组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombaatExtra", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> ProjectileMesh;

	// 抛射物的NiagaraSystem组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombaatExtra", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> ProjectileNS;

#pragma endregion


#pragma region ProjectileSettings

public:
	// 抛射物的ID
	UPROPERTY(Transient, BlueprintReadWrite, Category = "CombaatExtra")
	FName ProjectileID = NAME_None;

	// 抛射物的能力系统组件
	UPROPERTY(Transient, BlueprintReadWrite, Category = "CombaatExtra")
	TObjectPtr<UAbilitySystemComponent> InstigatorASC;

	// 投射物的外观
	UPROPERTY(Transient, BlueprintReadWrite, Category = "CombaatExtra")
	FCombatExtraSkin ProjectileSkin;

	// 投射物的效果
	UPROPERTY(Transient, BlueprintReadWrite, Category = "CombaatExtra")
	FCombatProjectileEffect ProjectileEffect;

	// 抛射物生成时的初始位置
	UPROPERTY(Transient, BlueprintReadWrite, Category = "CombaatExtra")
	TObjectPtr<AActor> TargetActor;

	// 抛射物生成时的初始位置
	UPROPERTY(Transient, BlueprintReadWrite, Category = "CombaatExtra")
	FVector TargetLocation;

	// 抛射物生成时的初始位置
	UPROPERTY(Transient, BlueprintReadWrite, Category = "CombaatExtra")
	float ExplodingRange;

	// 抛射物生成时的初始位置
	UPROPERTY(Transient, BlueprintReadWrite, Category = "CombaatExtra")
	FVector OriginLocation = FVector::ZeroVector;

#pragma endregion


#pragma region ProjectileFunctions

protected:
	void InitProjectileSkin() const;

public:
	UFUNCTION(BlueprintCallable, Category = "CombatExtra")
	void InitProjectileOnRespawn(UAbilitySystemComponent* OwnerASC, AActor* InTargetActor,
		FVector InTargetLocation, float InExplodingRange = 0.f);
	
	// 开始发射
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombaatExtra")
	void StartShoot();
	virtual void StartShoot_Implementation() {}

	// 结束发射
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombaatExtra")
	void FinishShoot();
	virtual void FinishShoot_Implementation();

	// 把效果应用到目标上
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombaatExtra")
	void ApplyEffectToTarget(AActor* InTarget);
	virtual void ApplyEffectToTarget_Implementation(AActor* InTarget);

#pragma endregion
};
