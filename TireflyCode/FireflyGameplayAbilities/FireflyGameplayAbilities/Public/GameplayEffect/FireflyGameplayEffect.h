// Copyright tzlFirefly, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "FireflyGameplayEffect.generated.h"


class UFireflyAbilitySystemCustomExecution;


// 属性与其对应的SetByCallerTag设置的结构体
USTRUCT()
struct FTireflySetByCallerTagConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute AttributeType = FGameplayAttribute();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SetByCallerTag = FGameplayTag();
};


// 基于GAS扩展的游戏性效果，在GameplayEffect的基础上进行了如下扩展：
// * 在属性更改的各个阶段加入跟随GameplayEffect的，可灵活插拔的自定义执行逻辑
//
// GameplayEffect extended from the GAS (Gameplay Ability System) that provides the following enhancements:
// * Pluggable custom execution logic for various stages of attribute changes
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyGameplayEffect: public UGameplayEffect
{
	GENERATED_BODY()

#pragma region GameplayEffect

public:
	UFireflyGameplayEffect(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	/** UObject interface */
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma endregion


#pragma region GameplayEffect_SetByCaller

protected:
	void ProcessDurationSetByCaller();
	
	void ProcessModiferSetByCaller();

#pragma endregion


#pragma region CustomExecution

public:
	// 自定义执行逻辑类的数组，GAS会以类默认对象的的方式调用这些类中的自定义执行逻辑
	// An array of custom logic classes, which GAS will call the custom execution logic from these classes in the form of class default objects.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CustomExecution)
	TArray<TSubclassOf<UFireflyAbilitySystemCustomExecution>> CustomExecutionClasses;

#pragma endregion
};
