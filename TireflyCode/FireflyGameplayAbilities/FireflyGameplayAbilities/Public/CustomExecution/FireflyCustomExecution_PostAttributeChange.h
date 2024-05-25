// Copyright tzlFirefly, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FireflyAbilitySystemCustomExecution.h"
#include "FireflyCustomExecution_PostAttributeChange.generated.h"


struct FGameplayAttribute;


// 自定义执行逻辑：在属性当前值发生修改后调用
// * 仅限通过函数 FGameplayAttribute::SetNumericValueChecked() 对属性当前值进行的修改能够触发该自定义执行逻辑
//
// Custom Execution Logic: Called after modifications to the current value of the attribute occur.
// * This custom execution logic is triggered exclusively when modifications to the current value of the attribute are made through the function FGameplayAttribute::SetNumericValueChecked().
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyCustomExecution_PostAttributeChange : public UFireflyAbilitySystemCustomExecution
{
	GENERATED_BODY()

public:
	// 自定义执行逻辑：在属性当前值发生修改后调用
	// * 仅限通过函数 FGameplayAttribute::SetNumericValueChecked() 对属性当前值进行的修改能够触发该自定义执行逻辑
	//
	// Custom Execution Logic: Called after modifications to the current value of the attribute occur.
	// * This custom execution logic is triggered exclusively when modifications to the current value of the attribute are made through the function FGameplayAttribute::SetNumericValueChecked().
	UFUNCTION(BlueprintNativeEvent, Category = CustomExecution)
	void HandlePostAttributeChange(UFireflyAbilitySystemComponent* FireflyASC
		, const FGameplayAttribute& Attribute, float OldValue, float NewValue);
	virtual void HandlePostAttributeChange_Implementation(UFireflyAbilitySystemComponent* FireflyASC
		, const FGameplayAttribute& Attribute, float OldValue, float NewValue) {}
	
};
