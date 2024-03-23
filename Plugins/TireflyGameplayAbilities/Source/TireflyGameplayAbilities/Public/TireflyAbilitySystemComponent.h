// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TireflyAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

#pragma region AbilitySystemComponent

public:
	UTireflyAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);

#pragma endregion


#pragma region AbilityActorInfo

public:
	// 初始化能力系统组件的ActorInfo，只能在本地客户端或服务端调用，在模拟端调用无效
	UFUNCTION(BlueprintCallable, Category = "Ability System", Meta = (DisplayName = "Init Ability Actor Info"))
	void K2_InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor);

#pragma endregion
};
