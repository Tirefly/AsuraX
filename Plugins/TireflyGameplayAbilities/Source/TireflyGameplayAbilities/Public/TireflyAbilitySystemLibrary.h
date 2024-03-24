// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayEffectTypes.h"
#include "TireflyAbilitySystemLibrary.generated.h"

class UTireflyAbilitySystemComponent;

/**
 * 
 */
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


#pragma region GameplayAttribute

public:
	// 检查游戏属性的名称和字符串是否不相等，用于实现“Switch on Gameplay Attribute”节点
	UFUNCTION(BlueprintPure, Category = PinOptions, meta = (BlueprintInternalUseOnly = "TRUE"))
	static bool NotEqual_AttributeAttribute(FGameplayAttribute A, FString B);

	// 获取属性被应用修改后在范围设置内的新的当前值，以及最小范围值和最大范围值
	UFUNCTION(BlueprintPure, Category = "Ability|Attribute")
	static bool GetAttributeValueInRange(const UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute
		, float ValueToClamp, float& NewValue, float& MinValue, float& MaxValue);

#pragma endregion
};
