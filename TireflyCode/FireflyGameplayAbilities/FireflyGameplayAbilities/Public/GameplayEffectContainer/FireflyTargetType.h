// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FireflyGameplayEffectContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "FireflyTargetType.generated.h"


/**
 * 用于确定能力目标的类
 * 意在通过蓝图运行目标逻辑
 * 这不是GameplayAbilityTargetActor的子类，因为此类永远不会实例化到世界中
 * 可以作为特定游戏目标蓝图的基础
 * 如果您的目标更复杂，则可能需要一次性实例化到世界中或作为与一个对象池Actor
 *
 * Class that is used to determine targeting for abilities
 * It is meant to be blueprinted to run target logic
 * This does not subclass GameplayAbilityTargetActor because this class is never instanced into the world
 * This can be used as a basis for a game-specific targeting blueprint
 * If your targeting is more complicated you may need to instance into the world once or as a pooled actor
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class FIREFLYGAMEPLAYABILITIES_API UFireflyTargetType : public UObject
{
	GENERATED_BODY()
	
public:
	UFireflyTargetType() {}

	// 用于确定应用游戏效果的目标
	// Called to determine targets to apply gameplay effects to
	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(AActor* Target, FGameplayEventData EventData
		, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
	virtual void GetTargets_Implementation(AActor* Target, FGameplayEventData EventData
		, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const {}
};


// 使用拥有者的简单目标类型
// Trivial target type that uses the owner
UCLASS(NotBlueprintable)
class FIREFLYGAMEPLAYABILITIES_API UFireflyTargetType_UseOwner : public UFireflyTargetType
{
	GENERATED_BODY()

public:
	UFireflyTargetType_UseOwner() {}

	virtual void GetTargets_Implementation(AActor* Target, FGameplayEventData EventData
		, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};


// 使用事件数据中提取目标的简单目标类型
// Trivial target type that pulls the target out of the event data
UCLASS(NotBlueprintable)
class FIREFLYGAMEPLAYABILITIES_API UFireflyTargetType_UseEventData : public UFireflyTargetType
{
	GENERATED_BODY()

public:
	UFireflyTargetType_UseEventData() {}

	virtual void GetTargets_Implementation(AActor* Target, FGameplayEventData EventData
		, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};