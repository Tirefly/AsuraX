// Copyright tzlFirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "FireflyAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

#pragma region AbilitySystemComponent

public:
	UFireflyAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);
	
#pragma endregion


#pragma region AbilityActorInfo

public:
	// 获取能力使用者的等级
	UFUNCTION(BlueprintCallable, Category = Abilities)
	virtual int32 GetAbilityActorLevel() const { return 1; }

#pragma endregion
};
