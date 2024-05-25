// Copyright tzlFirefly, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FireflyAbilitySystemCustomExecution.h"
#include "FireflyCustomExecution_PreAttributeBaseChange.generated.h"


struct FGameplayAttribute;


// 自定义执行逻辑：在属性基础值发生修改前调用
// * 仅限通过函数 FActiveGameplayEffectsContainer::SetAttributeBaseValue() 对属性基础值进行的修改能够触发该自定义执行逻辑
//
// Custom Execution Logic: Called before modifications to the base value of the attribute occur.
// * This custom execution logic is triggered exclusively when modifications to the base value of the attribute are made through the function FActiveGameplayEffectsContainer::SetAttributeBaseValue().
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyCustomExecution_PreAttributeBaseChange : public UFireflyAbilitySystemCustomExecution
{
	GENERATED_BODY()

public:
	// 自定义执行逻辑：在属性基础值发生修改前调用
	// * 仅限通过函数 FActiveGameplayEffectsContainer::SetAttributeBaseValue() 对属性基础值进行的修改能够触发该自定义执行逻辑
	//
	// Custom Execution Logic: Called before modifications to the base value of the attribute occur.
	// * This custom execution logic is triggered exclusively when modifications to the base value of the attribute are made through the function FActiveGameplayEffectsContainer::SetAttributeBaseValue().
	UFUNCTION(BlueprintNativeEvent, Category = CustomExecution)
	void HandlePreAttributeBaseChange(UFireflyAbilitySystemComponent* FireflyASC
		, const FGameplayAttribute& Attribute, const float& InNewValue, float& OutNewValue);
	virtual void HandlePreAttributeBaseChange_Implementation(UFireflyAbilitySystemComponent* FireflyASC
		, const FGameplayAttribute& Attribute, const float& InNewValue, float& OutNewValue) {}
};
