// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "FireflyGameplayEffectContainer.generated.h"


class UFireflyAbilitySystemComponent;
class UFireflyTargetType;
class UGameplayEffect;


/**
 * 结构体定义了一个游戏效果列表、一个标签和目标信息。
 * 这些容器在蓝图或资源中静态定义，然后在运行时变成Specs
 *
 * Struct defining a list of gameplay effects, a tag, and targeting info
 * These containers are defined statically in blueprints or assets and then turn into Specs at runtime
 */
USTRUCT(BlueprintType)
struct FFireflyGameplayEffectContainer
{
	GENERATED_BODY()

public:
	// 设置目标达成的方式
	// Sets the way that targeting happens
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TSubclassOf<UFireflyTargetType> TargetType;

	// 应用于目标的的游戏性效果列表
	// List of gameplay effects to apply to the targets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<TSubclassOf<UGameplayEffect>> TargetGameplayEffectClasses;

	FFireflyGameplayEffectContainer() {}
};


// FireflyGameplayEffectContainer的一个“处理过”的版本，可以将其传递并最终应用
// A "processed" version of FireflyGameplayEffectContainer that can be passed around and eventually applied
USTRUCT(BlueprintType)
struct FFireflyGameplayEffectContainerSpec
{
	GENERATED_BODY()

public:
	// 计算过的目标数据
	// Computed target data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	FGameplayAbilityTargetDataHandle TargetData;

	// 应用于目标的的游戏性效果列表
	// List of gameplay effects to apply to the targets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

	FFireflyGameplayEffectContainerSpec() {}

	// 如果此实例具有任何有效的效果规范，则返回true
	// Returns true if this has any valid effect specs
	bool HasValidEffects() const;

	// 如果此实例具有任何有效的目标，则返回true
	// Returns true if this has any valid targets
	bool HasValidTargets() const;

	// 将新目标添加到目标数据中
	// Adds new targets to target data
	void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);
};
