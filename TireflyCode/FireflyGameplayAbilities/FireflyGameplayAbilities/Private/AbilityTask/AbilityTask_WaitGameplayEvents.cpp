// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "AbilityTask/AbilityTask_WaitGameplayEvents.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(AbilityTask_WaitGameplayEvents)


UAbilityTask_WaitGameplayEvents::UAbilityTask_WaitGameplayEvents(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UAbilityTask_WaitGameplayEvents* UAbilityTask_WaitGameplayEvents::WaitGameplayEvents(UGameplayAbility* OwningAbility,
	const FGameplayTagContainer& EventTags, AActor* OptionalExternalTarget)
{
	UAbilityTask_WaitGameplayEvents* MyObj = NewAbilityTask<UAbilityTask_WaitGameplayEvents>(OwningAbility);

	MyObj->Tags = EventTags;
	MyObj->SetExternalTarget(OptionalExternalTarget);

	return MyObj;
}

void UAbilityTask_WaitGameplayEvents::Activate()
{
	Super::Activate();

	if (UAbilitySystemComponent* ASC = GetTargetASC())
	{
		TArray<FGameplayTag> TagArray;
		Tags.GetGameplayTagArray(TagArray);
		for (const FGameplayTag Tag : TagArray)
		{
			EventHandles.Add(Tag, ASC->GenericGameplayEventCallbacks.FindOrAdd(Tag).AddUObject(this, &UAbilityTask_WaitGameplayEvents::GameplayEventCallback));
		}
	}

	Super::Activate();
}

void UAbilityTask_WaitGameplayEvents::GameplayEventCallback(const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		EventReceived.Broadcast(Payload->EventTag, *Payload);
	}
}

void UAbilityTask_WaitGameplayEvents::OnDestroy(bool AbilityEnding)
{
	UAbilitySystemComponent* ASC = GetTargetASC();
	if (ASC && !EventHandles.IsEmpty())
	{
		for (auto EventHandle : EventHandles)
		{
			ASC->GenericGameplayEventCallbacks.FindOrAdd(EventHandle.Key).Remove(EventHandle.Value);
		}
	}

	Super::OnDestroy(AbilityEnding);
}

void UAbilityTask_WaitGameplayEvents::SetExternalTarget(AActor* Actor)
{
	if (Actor)
	{
		UseExternalTarget = true;
		OptionalExternalTarget = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
	}
}

UAbilitySystemComponent* UAbilityTask_WaitGameplayEvents::GetTargetASC()
{
	if (UseExternalTarget)
	{
		return OptionalExternalTarget;
	}

	return AbilitySystemComponent.Get();
}
