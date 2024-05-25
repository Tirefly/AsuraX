// Copyright tzlFirefly, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FireflyAbilitySystemCustomExecution.h"
#include "FireflyCustomExecution_PreGameplayEffectExecute.generated.h"


struct FGameplayEffectSpec;
struct FGameplayModifierEvaluatedData;
class UAbilitySystemComponent;


// 自定义执行逻辑：游戏性效果执行前调用
// * 游戏性效果的执行：GameplayEffect 的 DurationPolicy 是 Instant，或者 GameplayEffect 是周期性跳动的。
// * 游戏性效果的执行，会对属性的“基础值”造成不可逆的修改。
// 
// Custom Execution Logic: Called before the execution of Gameplay Effects.
// * Execution of Gameplay Effects: When the DurationPolicy of the GameplayEffect is Instant, or when the GameplayEffect is periodically ticking.
// * Execution of Gameplay Effects results in irreversible modifications to the "base value" of attributes.
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyCustomExecution_PreGameplayEffectExecute : public UFireflyAbilitySystemCustomExecution
{
	GENERATED_BODY()

public:
	// 自定义执行逻辑：游戏性效果执行前调用
	// * 游戏性效果的执行：GameplayEffect 的 DurationPolicy 是 Instant，或者 GameplayEffect 是周期性跳动的。
	// * 游戏性效果的执行，会对属性的“基础值”造成不可逆的修改。
	// 
	// Custom Execution Logic: Called before the execution of Gameplay Effects.
	// * Execution of Gameplay Effects: When the DurationPolicy of the GameplayEffect is Instant, or when the GameplayEffect is periodically ticking.
	// * Execution of Gameplay Effects results in irreversible modifications to the "base value" of attributes.
	UFUNCTION(BlueprintNativeEvent, Category = CustomExecution)
	void HandlePreGameplayEffectExecute(UAbilitySystemComponent* Source, UAbilitySystemComponent* Target
		, const FGameplayEffectSpec& EffectSpec, const FGameplayModifierEvaluatedData& InEvaluatedData, FGameplayModifierEvaluatedData& OutEvaluatedData);
	virtual void HandlePreGameplayEffectExecute_Implementation(UAbilitySystemComponent* Source, UAbilitySystemComponent* Target
		, const FGameplayEffectSpec& EffectSpec, const FGameplayModifierEvaluatedData& InEvaluatedData, FGameplayModifierEvaluatedData& OutEvaluatedData) {}

};