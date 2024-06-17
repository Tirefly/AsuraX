// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffectTypes.h"
#include "TireflyAbilitySystemLibrary.generated.h"


class UTireflyGameplayAbilityAsset;
class UTireflyGameplayAbility;
class UTireflyGameplayAbilityParameter;
class UTireflyAbilitySystemComponent;


/** 能力系统的辅助函数库 */
UCLASS()
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#pragma region AbilitySystem

public:
	// 尝试在Actor上找到一个TireflyAbilitySystemComponent
	UFUNCTION(BlueprintPure, Category = Ability)
	static UTireflyAbilitySystemComponent* GetTireflyAbilitySystemComponent(const AActor* Actor);

#pragma endregion


#pragma region AbilityParameter

public:
	// 获取Object作为Outer（或者Outermost）的AbilityAsset
	UFUNCTION(BlueprintPure, Category = "Ability")
	static UTireflyGameplayAbilityAsset* GetAbilityAsset(const UObject* InObject);
	
	// 根据AbilityRef获取指定的AbilityAsset参数（纯函数）
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAbilityClassPure(TSubclassOf<UTireflyGameplayAbility> AbilityClass, FName Parameter, bool& Success);

	// 根据AbilityRef获取指定的AbilityAsset参数（执行函数）
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAbilityClass(TSubclassOf<UTireflyGameplayAbility> AbilityClass, FName Parameter, bool& Success);
	
	// 根据AbilityID获取指定的AbilityAsset参数（纯函数）
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAbilityIdPure(FName AbilityId, FName Parameter, bool& Success);

	// 根据AbilityID获取指定的AbilityAsset参数（执行函数）
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAbilityId(FName AbilityId, FName Parameter, bool& Success);

	// 根据AssetRef获取指定的AbilityAsset参数（纯函数）
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAssetRefPure(UTireflyGameplayAbilityAsset* AssetRef, FName Parameter, bool& Success);

	// 根据AssetRef获取指定的AbilityAsset参数（执行函数）
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAssetRef(UTireflyGameplayAbilityAsset* AssetRef, FName Parameter, bool& Success);

	// 根据AssetPak获取指定的AbilityAsset参数（纯函数）
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAssetPakPure(FString AssetPak, FName Parameter, bool& Success);

	// 根据AssetPak获取指定的AbilityAsset参数（执行函数）
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Ability")
	static UTireflyGameplayAbilityParameter* GetAbilityParamOfAssetPak(FString AssetPak, FName Parameter, bool& Success);

#pragma endregion


#pragma region GameplayAbility

public:
	// 根据Ability细则的句柄获取Ability的类
	UFUNCTION(BlueprintPure, Category = "Ability")
	static TSubclassOf<UGameplayAbility> GetAbilityClassByHandle(const UAbilitySystemComponent* ASC, const FGameplayAbilitySpecHandle& Handle);

#pragma endregion


#pragma region GameplayAttribute

public:
	// 检查游戏属性的名称和字符串是否不相等，用于实现“Switch on Gameplay Attribute”节点
	UFUNCTION(BlueprintPure, Category = PinOptions, meta = (BlueprintInternalUseOnly = "TRUE"))
	static bool NotEqual_AttributeAttribute(const FGameplayAttribute& A, const FString& B) { return A.GetName() != B; }

	// 获取属性被应用修改后在范围设置内的新的当前值，以及最小范围值和最大范围值
	UFUNCTION(BlueprintPure, Category = "Ability|Attribute")
	static bool GetAttributeValueInRange(const AActor* Actor, FGameplayAttribute Attribute,
		float ValueToClamp, float& NewValue, FVector2D& OutRange);

#pragma endregion
};
