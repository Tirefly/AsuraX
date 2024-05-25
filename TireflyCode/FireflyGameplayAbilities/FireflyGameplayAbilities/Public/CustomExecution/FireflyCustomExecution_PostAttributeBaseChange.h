// Copyright tzlFirefly, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FireflyAbilitySystemCustomExecution.h"
#include "FireflyCustomExecution_PostAttributeBaseChange.generated.h"


struct FGameplayAttribute;


// 自定义执行逻辑：在属性基础值发生修改后调用
// * 仅限通过函数 FActiveGameplayEffectsContainer::SetAttributeBaseValue() 对属性基础值进行的修改能够触发该自定义执行逻辑
//
// Custom Execution Logic: Called after modifications to the base value of the attribute occur.
// * This custom execution logic is triggered exclusively when modifications to the base value of the attribute are made through the function FActiveGameplayEffectsContainer::SetAttributeBaseValue().
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyCustomExecution_PostAttributeBaseChange : public UFireflyAbilitySystemCustomExecution
{
	GENERATED_BODY()

public:
	// 自定义执行逻辑：在属性基础值发生修改后调用
	// * 仅限通过函数 FActiveGameplayEffectsContainer::SetAttributeBaseValue() 对属性基础值进行的修改能够触发该自定义执行逻辑
	//
	// Custom Execution Logic: Called after modifications to the base value of the attribute occur.
	// * This custom execution logic is triggered exclusively when modifications to the base value of the attribute are made through the function FActiveGameplayEffectsContainer::SetAttributeBaseValue().
	UFUNCTION(BlueprintNativeEvent, Category = CustomExecution)
	void HandlePostAttributeBaseChange(UFireflyAbilitySystemComponent* FireflyASC
		, const FGameplayAttribute& Attribute, float OldValue, float NewValue);
	virtual void HandlePostAttributeBaseChange_Implementation(UFireflyAbilitySystemComponent* FireflyASC
		, const FGameplayAttribute& Attribute, float OldValue, float NewValue) {}
	
};
