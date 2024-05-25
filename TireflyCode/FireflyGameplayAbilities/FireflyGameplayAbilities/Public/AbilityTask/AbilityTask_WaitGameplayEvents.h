// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "UObject/ObjectMacros.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilityTask_WaitGameplayEvents.generated.h"


class UAbilitySystemComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWaitGameplayEventsDelegate, const FGameplayTag&, EventTag, const FGameplayEventData&, Payload);


UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UAbilityTask_WaitGameplayEvents : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintAssignable)
	FWaitGameplayEventsDelegate	EventReceived;

	/**
	 * 等待指定的游戏标签事件之一被触发。
	 * 默认情况下，该任务将查看此能力的所有者的游戏性标签事件
	 * 可以通过设置OptionalExternalTarget来让该任务查看另一个Actor的标签事件
	 * 默认情况下，该任务只会监听 完全匹配的标签事件的触发
	 *
	 * Wait until one of the specified gameplay tag events is triggered.
	 * By default this will look at the owner of this ability.
	 * OptionalExternalTarget can be set to make this look at another actor's tags for changes
	 * By default this will only trigger for exactly matched event tags
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", Meta = (HidePin =  "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitGameplayEvents* WaitGameplayEvents(UGameplayAbility* OwningAbility
		, const FGameplayTagContainer& EventTags, AActor* OptionalExternalTarget = nullptr);

	virtual void Activate() override;

	virtual void GameplayEventCallback(const FGameplayEventData* Payload);

	virtual void OnDestroy(bool AbilityEnding) override;

	void SetExternalTarget(AActor* Actor);

	UAbilitySystemComponent* GetTargetASC();

protected:
	FGameplayTagContainer Tags;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OptionalExternalTarget;

	bool UseExternalTarget;

	TMap<FGameplayTag, FDelegateHandle> EventHandles;
};
