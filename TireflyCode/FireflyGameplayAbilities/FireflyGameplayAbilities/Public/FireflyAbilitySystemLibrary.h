// Copyright tzlFirefly, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "FireflyAbilitySystemLibrary.generated.h"


class UGameplayEffectUIData;
class UFireflyAbilitySystemComponent;
class UFireflyGameplayAbilityAsset;
class UFireflyGameplayAbility;
class UTireflyGameplayAbilityParameter;
struct FGameplayAttribute;


// GAS的蓝图函数库，这些功能中有许多对于从本地调用也是有用的
// Blueprint function library for gameplay ability system. Many of these functions are useful to call from native as well
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#pragma region AbilitySystem

public:
	// 尝试在Actor上找到一个FireflyAbilitySystemComponent
	// Tries to find a firefly ability system component on the actor
	UFUNCTION(BlueprintPure, Category = Ability)
	static UFireflyAbilitySystemComponent* GetFireflyAbilitySystemComponent(const AActor* Actor);

#pragma endregion


#pragma region GameplayAttribute

public:
	// 检查游戏属性的名称和字符串是否不相等
	// Checks if a gameplay attribute's name and a string are not equal to one another
	UFUNCTION(BlueprintPure, Category = PinOptions, meta = (BlueprintInternalUseOnly = "TRUE"))
	static bool NotEqual_AttributeAttribute(FGameplayAttribute A, FString B);

	// 获取属性被应用修改后在范围设置内的新的当前值，以及最小范围值和最大范围值
	UFUNCTION(BlueprintPure, Category = "Ability|Attribute")
	static bool GetAttributeValueInRange(const AActor* Actor, FGameplayAttribute Attribute,
		float ValueToClamp, float& NewValue, float& RangeMin, float& RangeMax);

#pragma endregion


#pragma region GameplayEffect_Magnitude

public:
	static float CalculateMathExpression(const FString& Expression);

#pragma endregion


#pragma region GameplayEffect_ActiveHandle

public:
	// 判断给定的活跃游戏性效果句柄是否有效
	// Return if the specific active gameplay effect handle is valid
	UFUNCTION(BlueprintPure, Category = GameplayEffect)
	static bool IsActiveGameplayEffectHandleValid(const FActiveGameplayEffectHandle& Handle);

	// 判定给定的活跃游戏性效果句柄是否包含指定的资产标签
	// Return if the specific active gameplay effect handle contains the asset tag
	UFUNCTION(BlueprintPure, Category = GameplayEffect)
	static bool ActiveGameplayEffectHandleHasAssetTag(const FActiveGameplayEffectHandle& Handle, FGameplayTag Tag);

	// 判定给定的活跃游戏性效果句柄是否包含指定的赋予标签
	// Return if the specific active gameplay effect handle contains the granted tag
	UFUNCTION(BlueprintPure, Category = GameplayEffect)
	static bool ActiveGameplayEffectHandleHasGrantedTag(const FActiveGameplayEffectHandle& Handle, FGameplayTag Tag);

	// 获取活跃游戏效果的UI数据
	// Get active gameplay effect 's UI data
	UFUNCTION(BlueprintCallable, Category = GameplayEffect, Meta = (DisplayName = "Get Active Gameplay Effect UI Data", DeterminesOutputType = "DataType"))
	static const UGameplayEffectUIData* GetActiveGameplayEffectUIData(const FActiveGameplayEffectHandle& Handle, TSubclassOf<UGameplayEffectUIData> DataType);

#pragma endregion


#pragma region GameplayAbility

public:
	// 判断给定的游戏性能力句柄是否有效
	// Return if the specific gameplay ability handle is valid
	UFUNCTION(BlueprintPure, Category = GameplayEffect)
	static bool IsGameplayAbilityHandleValid(const FGameplayAbilitySpecHandle& Handle);

#pragma endregion


#pragma region AbilityParameter

public:
	// 根据AbilityRef获取指定的AbilityAsset参数（纯函数）
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAbilityClassPure(TSubclassOf<UFireflyGameplayAbility> AbilityClass, FName Parameter, bool& Success);

	// 根据AbilityRef获取指定的AbilityAsset参数（执行函数）
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAbilityClass(TSubclassOf<UFireflyGameplayAbility> AbilityClass, FName Parameter, bool& Success);
	
	// 根据AbilityID获取指定的AbilityAsset参数（纯函数）
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAbilityIdPure(FName AbilityId, FName Parameter, bool& Success);

	// 根据AbilityID获取指定的AbilityAsset参数（执行函数）
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAbilityId(FName AbilityId, FName Parameter, bool& Success);

	// 根据AssetRef获取指定的AbilityAsset参数（纯函数）
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAssetRefPure(UFireflyGameplayAbilityAsset* AssetRef, FName Parameter, bool& Success);

	// 根据AssetRef获取指定的AbilityAsset参数（执行函数）
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAssetRef(UFireflyGameplayAbilityAsset* AssetRef, FName Parameter, bool& Success);

	// 根据AssetPak获取指定的AbilityAsset参数（纯函数）
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAssetPakPure(FString AssetPak, FName Parameter, bool& Success);

	// 根据AssetPak获取指定的AbilityAsset参数（执行函数）
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAssetPak(FString AssetPak, FName Parameter, bool& Success);

#pragma endregion


#pragma region CustomExecution

public:
	// 在蓝图端展开一个游戏性效果的属性修改器估值数据
	// Break an GameplayEffect's modifier evaluated data in blueprint
	UFUNCTION(BlueprintPure, Category = GameplayEffect)
	static bool GetModifierEvaluatedData(const FGameplayModifierEvaluatedData& InEvaluatedData, FGameplayAttribute& OutAttribute
		, TEnumAsByte<EGameplayModOp::Type>& OutModifierOp, float&  OutMagnitude, FActiveGameplayEffectHandle& OutHandle);

	// 在蓝图端展开一个游戏性效果的属性修改器估值数据
	// Break an GameplayEffect's modifier evaluated data in blueprint
	UFUNCTION(BlueprintPure, Category = GameplayEffect)
	static FGameplayModifierEvaluatedData MakeModifierEvaluatedData(const FGameplayAttribute& Attribute
		, TEnumAsByte<EGameplayModOp::Type> ModifierOp, float Magnitude, const FActiveGameplayEffectHandle& Handle);

#pragma endregion


#pragma region GameplayTag

public:
	// 检测Actor的能力系统组件是否存在匹配的游戏性标签
	// Check if Actor's AbilitySystemComponent has matching gameplay tag.
	UFUNCTION(BlueprintPure, Category = GameplayTags, Meta = (DefaultToSelf = "InActor"))
	static bool ActorHasMatchingGameplayTag(const AActor* InActor, FGameplayTag InTag);

#pragma endregion
};
