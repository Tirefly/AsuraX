// Copyright tzlFirefly, 2023. All Rights Reserved.


#include "FireflyAbilitySystemComponent.h"

#include "AbilitySystemLog.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemStats.h"
#include "GameplayAttribute/FireflyAttributeSet.h"

#include "GameplayAbility/FireflyGameplayAbility.h"
#include "GameplayEffect/FireflyGameplayEffect.h"
#include "CustomExecution/FireflyAbilitySystemCustomExecution.h"

#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"


UFireflyAbilitySystemComponent::UFireflyAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UFireflyAbilitySystemComponent::BeginDestroy()
{
	ShutdownGameplayEffectDelegates();
	ShutdownGameplayTagDelegates();

	for (TObjectPtr<UAttributeSet> AttributeSet : GetSpawnedAttributes())
	{
		ShutdownGameplayAttributeDelegates(Cast<UFireflyAttributeSet>(AttributeSet.Get()));
	}

	if (IsValid(CustomExecutionContainer))
	{
		CustomExecutionContainer->MarkAsGarbage();
	}

	Super::BeginDestroy();
}

void UFireflyAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	CustomExecutionContainer = NewObject<UFireflyCustomExecutionContainer>(InOwnerActor);
	RegisterGameplayEffectDelegates();
	RegisterGameplayTagDelegates();
}

void UFireflyAbilitySystemComponent::K2_InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		UE_LOG(LogAbilitySystem, Warning, TEXT("UFireflyAbilitySystemComponent::K2_InitAbilityActorInfo: Can't call this from a simulated proxy actor."));
		return;
	}
	
	InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}

FGameplayAbilitySpec* UFireflyAbilitySystemComponent::FindAbilitySpecFromHandle(FGameplayAbilitySpecHandle Handle) const
{
	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Handle == Handle)
		{
			return const_cast<FGameplayAbilitySpec*>(&Spec);
		}
	}

	return nullptr;
}

UEnhancedInputComponent* UFireflyAbilitySystemComponent::GetOwnerEnhancedInputComponent() const
{
	if (const APawn* PawnAvatar = Cast<APawn>(GetAvatarActor()))
	{
		return Cast<UEnhancedInputComponent>(PawnAvatar->InputComponent);
	}

	return nullptr;
}

bool UFireflyAbilitySystemComponent::BindAbilityToInputAction(UInputAction* InputToBind,
	TSubclassOf<UFireflyGameplayAbility> AbilityToBind)
{
	const FString FuncName = TEXT("UFireflyAbilitySystemComponent::BindAbilityToInputAction()");
	if (!IsValid(InputToBind) || !IsValid(AbilityToBind))
	{
		return false;
	}

	// 判定技能系统组件的Avatar是否被玩家控制
	UEnhancedInputComponent* EnhancedInput = GetOwnerEnhancedInputComponent();
	if (!IsValid(EnhancedInput))
	{
		UE_LOG(LogAbilitySystem, Warning, TEXT("%s AbilitySystemComponent's AvatarActor is not controlled by Player"), *FuncName);
		return false;
	}

	// 判定技能是否已经被赋予
	const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityToBind);
	if (!AbilitySpec)
	{
		UE_LOG(LogAbilitySystem, Warning, TEXT("%s Ability %s has not been granted by AbilitySystemComponent"), *FuncName, *AbilityToBind->GetName());
		return false;
	}

	FFireflyAbilityInputBindingData* NewBindingData = AbilityInputBindings.Find(InputToBind);

	// 输入的绑定信息是否已经存在
	if (!NewBindingData)
	{
		// 若输入的绑定信息尚不存在，构建一个并将技能和输入绑定
		NewBindingData = new FFireflyAbilityInputBindingData();
		NewBindingData->Abilities.Emplace(AbilitySpec->Handle);
		// Bind Started Event
		NewBindingData->HandleStarted = EnhancedInput->BindAction(InputToBind, ETriggerEvent::Started
			, this, &UFireflyAbilitySystemComponent::AbilityLocalInputStarted, InputToBind).GetHandle();
		// Bind Ongoing Event
		NewBindingData->HandleOngoing = EnhancedInput->BindAction(InputToBind, ETriggerEvent::Ongoing
			, this, &UFireflyAbilitySystemComponent::AbilityLocalInputOngoing, InputToBind).GetHandle();
		// Bind Canceled Event
		NewBindingData->HandleCanceled = EnhancedInput->BindAction(InputToBind, ETriggerEvent::Canceled
			, this, &UFireflyAbilitySystemComponent::AbilityLocalInputCanceled, InputToBind).GetHandle();
		// Bind Triggered Event
		NewBindingData->HandleTriggered = EnhancedInput->BindAction(InputToBind, ETriggerEvent::Triggered
			, this, &UFireflyAbilitySystemComponent::AbilityLocalInputTriggered, InputToBind).GetHandle();
		// Bind Completed Event
		NewBindingData->HandleCompleted = EnhancedInput->BindAction(InputToBind, ETriggerEvent::Completed
			, this, &UFireflyAbilitySystemComponent::AbilityLocalInputCompleted, InputToBind).GetHandle();

		AbilityInputBindings.Add(InputToBind, *NewBindingData);
		return true;
	}

	// 判定技能是否已经与输入绑定
	if (NewBindingData->Abilities.Find(AbilitySpec->Handle) != INDEX_NONE)
	{
		UE_LOG(LogAbilitySystem, Display, TEXT("%s Ability %s has already bound to Input %s"), *FuncName, *AbilityToBind->GetName(), *InputToBind->GetName());
		return false;
	}

	NewBindingData->Abilities.Emplace(AbilitySpec->Handle);
	return true;
}

void UFireflyAbilitySystemComponent::UnbindAbilityFromInputAction(UInputAction* InputToUnbind,
	TSubclassOf<UFireflyGameplayAbility> AbilityToUnbind)
{
	const FString FuncName = TEXT("UFireflyAbilitySystemComponent::UnbindAbilityFromInputAction()");
	if (!IsValid(InputToUnbind) || !IsValid(AbilityToUnbind))
	{
		return;
	}

	// 判定技能系统组件的Avatar是否被玩家控制
	UEnhancedInputComponent* EnhancedInput = GetOwnerEnhancedInputComponent();
	if (!IsValid(EnhancedInput))
	{
		UE_LOG(LogAbilitySystem, Warning, TEXT("%s AbilitySystemComponent's AvatarActor is not controlled by Player"), *FuncName);
		return;
	}

	// 判定技能是否已经被赋予
	const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityToUnbind);
	if (!AbilitySpec)
	{
		UE_LOG(LogAbilitySystem, Display, TEXT("%s Ability %s has not been granted by AbilitySystemComponent"), *FuncName, *AbilityToUnbind->GetName());
		return;
	}

	// 判定技能是否已经与输入绑定
	FFireflyAbilityInputBindingData* BindingDataToModify = AbilityInputBindings.Find(InputToUnbind);
	if (BindingDataToModify->Abilities.Find(AbilitySpec->Handle) == INDEX_NONE)
	{
		UE_LOG(LogAbilitySystem, Display, TEXT("%s Ability %s has not been bound to Input %s"), *FuncName, *AbilityToUnbind->GetName(), *InputToUnbind->GetName());
		return;
	}

	// 将技能从输入绑定中移除
	BindingDataToModify->Abilities.RemoveSingleSwap(AbilitySpec->Handle);
	if (BindingDataToModify->Abilities.Num() > 0)
	{
		return;
	}

	// 若输入已经不再绑定任何技能，将输入从绑定信息中移除
	EnhancedInput->RemoveBindingByHandle(BindingDataToModify->HandleStarted);
	EnhancedInput->RemoveBindingByHandle(BindingDataToModify->HandleOngoing);
	EnhancedInput->RemoveBindingByHandle(BindingDataToModify->HandleCanceled);
	EnhancedInput->RemoveBindingByHandle(BindingDataToModify->HandleTriggered);
	EnhancedInput->RemoveBindingByHandle(BindingDataToModify->HandleCompleted);
	AbilityInputBindings.Remove(InputToUnbind);
}

void UFireflyAbilitySystemComponent::AbilityLocalInputStarted(const FInputActionValue& InputValue,
	UInputAction* Input)
{
	ABILITYLIST_SCOPE_LOCK();
	// 根据输入从技能系统中找到对应的技能输入绑定信息
	FFireflyAbilityInputBindingData* BindingData = AbilityInputBindings.Find(Input);
	// 遍历输入绑定的所有技能的规范句柄
	for (const auto& SpecHandle : BindingData->Abilities)
	{
		// 技能规范句柄是否有效的代表一个技能规范
		FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(SpecHandle);
		if (!Spec)
		{
			continue;
		}

		// 技能规范的实例是否有效
		if (!IsValid(Spec->Ability))
		{
			continue;
		}

		// 无论技能是否处于激活状态，Started事件都应该让技能规范的输入状态更新到Pressed状态
		Spec->InputPressed = true;

		// 若技能未处于激活状态
		if (!Spec->IsActive())
		{
			if (const UFireflyGameplayAbility* FireflyGA = Cast<UFireflyGameplayAbility>(Spec->Ability))
			{
				// 该技能可被输入的Started事件激活，则尝试激活技能
				if ((FireflyGA->InputActivationPolicy & FIREFLY_TO_FLAG(EFireflyAbilityInputActivationPolicy::OnInputStarted)))
				{
					TryActivateAbility(SpecHandle);
				}
			}

			// 不执行技能内部的输入事件和技能输入事件的网络同步
			continue;
		}

		// 如果允许直接复制输入事件，并且当前权限不属于服务端，则向服务端发送输入Press事件和输入Started事件
		if (Spec->Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
		{
			ServerSetInputPressed(SpecHandle);
			Server_AbilityInputStarted(SpecHandle, InputValue.GetValueType(), InputValue.Get<FVector>());
		}

		// 通知技能规范执行技能内部的输入事件：Pressed
		AbilitySpecInputPressed(*Spec);
		// 通知技能规范执行技能内部的输入事件：Started
		AbilitySpecInputStarted(*Spec, InputValue);

		// 触发技能系统的通用InputPressed事件，此处不进行复制。如果有人监听，他们可以将InputPressed事件复制到服务器端
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, SpecHandle, Spec->ActivationInfo.GetActivationPredictionKey());
	}
}

void UFireflyAbilitySystemComponent::AbilityLocalInputOngoing(const FInputActionValue& InputValue,
	UInputAction* Input)
{
	ABILITYLIST_SCOPE_LOCK();
	// 根据输入从技能系统中找到对应的技能输入绑定信息
	FFireflyAbilityInputBindingData* BindingData = AbilityInputBindings.Find(Input);
	// 遍历输入绑定的所有技能的规范句柄
	for (const auto& SpecHandle : BindingData->Abilities)
	{
		// 技能规范句柄是否有效的代表一个技能规范
		FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(SpecHandle);
		if (!Spec)
		{
			continue;
		}

		// 技能规范的实例是否有效
		if (!IsValid(Spec->Ability))
		{
			continue;
		}

		// 若技能未处于激活状态，不执行技能内部的输入事件和技能输入事件的网络同步
		if (!Spec->IsActive())
		{
			continue;
		}

		// 如果允许直接复制输入事件，并且当前权限不属于服务端，则向服务端发送输入Ongoing事件
		if (Spec->Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
		{
			Server_AbilityInputOngoing(SpecHandle, InputValue.GetValueType(), InputValue.Get<FVector>());
		}

		// 通知技能规范执行技能内部的输入事件：Ongoing
		AbilitySpecInputOngoing(*Spec, InputValue);

		// Ongoing输入事件不同于Started、Triggered、Canceled、Completed，不会触发技能系统的通用事件
	}
}

void UFireflyAbilitySystemComponent::AbilityLocalInputCanceled(const FInputActionValue& InputValue,
	UInputAction* Input)
{
	ABILITYLIST_SCOPE_LOCK();
	// 根据输入从技能系统中找到对应的技能输入绑定信息
	FFireflyAbilityInputBindingData* BindingData = AbilityInputBindings.Find(Input);
	// 遍历输入绑定的所有技能的规范句柄
	for (const auto& SpecHandle : BindingData->Abilities)
	{
		// 技能规范句柄是否有效的代表一个技能规范
		FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(SpecHandle);
		if (!Spec)
		{
			continue;
		}

		// 无论技能是否处于激活状态，Canceled事件都应该让技能规范的输入状态更新到Released状态
		Spec->InputPressed = false;

		// 如果技能规范的实例无效，或技能未处于激活状态，不执行技能内部的输入事件和技能输入事件的网络同步
		if (!IsValid(Spec->Ability) || !Spec->IsActive())
		{
			continue;
		}

		// 如果允许直接复制输入事件，并且当前权限不属于服务端，则向服务端发送输入Release事件和输入Canceled事件
		if (Spec->Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
		{
			ServerSetInputReleased(SpecHandle);
			Server_AbilityInputCanceled(SpecHandle, InputValue.GetValueType(), InputValue.Get<FVector>());
		}

		// 通知技能规范执行技能内部的输入事件：Released
		AbilitySpecInputReleased(*Spec);
		// 通知技能规范执行技能内部的输入事件：Canceled
		AbilitySpecInputCanceled(*Spec, InputValue);

		// 触发技能系统的通用InputPressed事件，此处不进行复制。如果有人监听，他们可以将InputPressed事件复制到服务器端
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, SpecHandle, Spec->ActivationInfo.GetActivationPredictionKey());
	}
}

void UFireflyAbilitySystemComponent::AbilityLocalInputTriggered(const FInputActionValue& InputValue,
	UInputAction* Input)
{
	ABILITYLIST_SCOPE_LOCK();
	// 根据输入从技能系统中找到对应的技能输入绑定信息
	FFireflyAbilityInputBindingData* BindingData = AbilityInputBindings.Find(Input);
	// 遍历输入绑定的所有技能的规范句柄
	for (const auto& SpecHandle : BindingData->Abilities)
	{
		// 技能规范句柄是否有效的代表一个技能规范
		FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(SpecHandle);
		if (!Spec)
		{
			continue;
		}

		// 技能规范的实例是否有效
		if (!IsValid(Spec->Ability))
		{
			continue;
		}

		// 无论技能是否处于激活状态，Triggered事件都应该让技能规范的输入状态更新到Pressed状态
		Spec->InputPressed = true;

		// 若技能未处于激活状态
		if (!Spec->IsActive())
		{
			if (const UFireflyGameplayAbility* FireflyGA = Cast<UFireflyGameplayAbility>(Spec->Ability))
			{
				// 该技能可被输入的Triggered事件激活，则尝试激活技能
				if ((FireflyGA->InputActivationPolicy & FIREFLY_TO_FLAG(EFireflyAbilityInputActivationPolicy::OnInputTriggered)))
				{
					TryActivateAbility(SpecHandle);
				}
			}

			// 不执行技能内部的输入事件和技能输入事件的网络同步
			continue;
		}

		// 如果允许直接复制输入事件，并且当前权限不属于服务端，则向服务端发送输入Press事件和输入Triggered事件
		if (Spec->Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
		{
			ServerSetInputPressed(SpecHandle);
			Server_AbilityInputTriggered(SpecHandle, InputValue.GetValueType(), InputValue.Get<FVector>());
		}

		// 通知技能规范执行技能内部的输入事件：Pressed
		AbilitySpecInputPressed(*Spec);
		// 通知技能规范执行技能内部的输入事件：Triggered
		AbilitySpecInputTriggered(*Spec, InputValue);

		// 触发技能系统的通用InputPressed事件，此处不进行复制。如果有人监听，他们可以将InputPressed事件复制到服务器端
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, SpecHandle, Spec->ActivationInfo.GetActivationPredictionKey());
	}
}

void UFireflyAbilitySystemComponent::AbilityLocalInputCompleted(const FInputActionValue& InputValue,
	UInputAction* Input)
{
	ABILITYLIST_SCOPE_LOCK();
	// 根据输入从技能系统中找到对应的技能输入绑定信息
	FFireflyAbilityInputBindingData* BindingData = AbilityInputBindings.Find(Input);
	// 遍历输入绑定的所有技能的规范句柄
	for (const auto& SpecHandle : BindingData->Abilities)
	{
		// 技能规范句柄是否有效的代表一个技能规范
		FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(SpecHandle);
		if (!Spec)
		{
			continue;
		}

		// 无论技能是否处于激活状态，Completed事件都应该让技能规范的输入状态更新到Released状态
		Spec->InputPressed = false;

		// 如果技能规范的实例无效，或技能未处于激活状态，不执行技能内部的输入事件和技能输入事件的网络同步
		if (!IsValid(Spec->Ability) || !Spec->IsActive())
		{
			continue;
		}

		// 如果允许直接复制输入事件，并且当前权限不属于服务端，则向服务端发送输入Release事件和输入Completed事件
		if (Spec->Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
		{
			ServerSetInputReleased(SpecHandle);
			Server_AbilityInputCompleted(SpecHandle, InputValue.GetValueType(), InputValue.Get<FVector>());
		}

		// 通知技能规范执行技能内部的输入事件：Released
		AbilitySpecInputReleased(*Spec);
		// 通知技能规范执行技能内部的输入事件：Completed
		AbilitySpecInputCompleted(*Spec, InputValue);

		// 触发技能系统的通用InputPressed事件，此处不进行复制。如果有人监听，他们可以将InputPressed事件复制到服务器端
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, SpecHandle, Spec->ActivationInfo.GetActivationPredictionKey());
	}
}

void UFireflyAbilitySystemComponent::Server_AbilityInputStarted_Implementation(
	FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue)
{
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilitySpecHandle);
	if (Spec)
	{
		AbilitySpecInputStarted(*Spec, FInputActionValue(InputValueType, InputValue));
	}
}

bool UFireflyAbilitySystemComponent::Server_AbilityInputStarted_Validate(FGameplayAbilitySpecHandle AbilitySpecHandle,
	EInputActionValueType InputValueType, FVector InputValue)
{
	return true;
}

void UFireflyAbilitySystemComponent::Server_AbilityInputOngoing_Implementation(
	FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue)
{
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilitySpecHandle);
	if (Spec)
	{
		AbilitySpecInputOngoing(*Spec, FInputActionValue(InputValueType, InputValue));
	}
}

bool UFireflyAbilitySystemComponent::Server_AbilityInputOngoing_Validate(FGameplayAbilitySpecHandle AbilitySpecHandle,
	EInputActionValueType InputValueType, FVector InputValue)
{
	return true;
}

void UFireflyAbilitySystemComponent::Server_AbilityInputCanceled_Implementation(
	FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue)
{
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilitySpecHandle);
	if (Spec)
	{
		AbilitySpecInputCanceled(*Spec, FInputActionValue(InputValueType, InputValue));
	}
}

bool UFireflyAbilitySystemComponent::Server_AbilityInputCanceled_Validate(FGameplayAbilitySpecHandle AbilitySpecHandle,
	EInputActionValueType InputValueType, FVector InputValue)
{
	return true;
}

void UFireflyAbilitySystemComponent::Server_AbilityInputTriggered_Implementation(
	FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue)
{
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilitySpecHandle);
	if (Spec)
	{
		AbilitySpecInputTriggered(*Spec, FInputActionValue(InputValueType, InputValue));
	}
}

bool UFireflyAbilitySystemComponent::Server_AbilityInputTriggered_Validate(FGameplayAbilitySpecHandle AbilitySpecHandle,
	EInputActionValueType InputValueType, FVector InputValue)
{
	return true;
}

void UFireflyAbilitySystemComponent::Server_AbilityInputCompleted_Implementation(
	FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue)
{
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilitySpecHandle);
	if (Spec)
	{
		AbilitySpecInputCompleted(*Spec, FInputActionValue(InputValueType, InputValue));
	}
}

bool UFireflyAbilitySystemComponent::Server_AbilityInputCompleted_Validate(FGameplayAbilitySpecHandle AbilitySpecHandle,
	EInputActionValueType InputValueType, FVector InputValue)
{
	return true;
}

void UFireflyAbilitySystemComponent::AbilitySpecInputStarted(FGameplayAbilitySpec& Spec, const FInputActionValue& InputValue)
{
	if (Spec.IsActive())
	{
		// The ability is active, so just pipe the input event to it
		if (Spec.Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced)
		{
			if (UFireflyGameplayAbility* FireflyGA = Cast<UFireflyGameplayAbility>(Spec.Ability))
			{
				FireflyGA->OnAbilityInputStarted(InputValue);
				FireflyGA->OnAbilityInputEvent(ETriggerEvent::Started, InputValue);
			}
		}
		else
		{
			TArray<UGameplayAbility*> Instances = Spec.GetAbilityInstances();
			for (UGameplayAbility* Instance : Instances)
			{
				if (UFireflyGameplayAbility* FireflyGA = Cast<UFireflyGameplayAbility>(Instance))
				{
					FireflyGA->OnAbilityInputStarted(InputValue);
					FireflyGA->OnAbilityInputEvent(ETriggerEvent::Started, InputValue);
				}
			}
		}
	}
}

void UFireflyAbilitySystemComponent::AbilitySpecInputOngoing(FGameplayAbilitySpec& Spec, const FInputActionValue& InputValue)
{
	if (Spec.IsActive())
	{
		// The ability is active, so just pipe the input event to it
		if (Spec.Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced)
		{
			if (UFireflyGameplayAbility* FireflyGA = Cast<UFireflyGameplayAbility>(Spec.Ability))
			{
				FireflyGA->OnAbilityInputOngoing(InputValue);
				FireflyGA->OnAbilityInputEvent(ETriggerEvent::Ongoing, InputValue);
			}
		}
		else
		{
			TArray<UGameplayAbility*> Instances = Spec.GetAbilityInstances();
			for (UGameplayAbility* Instance : Instances)
			{
				if (UFireflyGameplayAbility* FireflyGA = Cast<UFireflyGameplayAbility>(Instance))
				{
					FireflyGA->OnAbilityInputOngoing(InputValue);
					FireflyGA->OnAbilityInputEvent(ETriggerEvent::Ongoing, InputValue);
				}
			}
		}
	}
}

void UFireflyAbilitySystemComponent::AbilitySpecInputCanceled(FGameplayAbilitySpec& Spec,
	const FInputActionValue& InputValue)
{
	if (Spec.IsActive())
	{
		// The ability is active, so just pipe the input event to it
		if (Spec.Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced)
		{
			if (UFireflyGameplayAbility* FireflyGA = Cast<UFireflyGameplayAbility>(Spec.Ability))
			{
				FireflyGA->OnAbilityInputCanceled(InputValue);
				FireflyGA->OnAbilityInputEvent(ETriggerEvent::Canceled, InputValue);
			}
		}
		else
		{
			TArray<UGameplayAbility*> Instances = Spec.GetAbilityInstances();
			for (UGameplayAbility* Instance : Instances)
			{
				if (UFireflyGameplayAbility* FireflyGA = Cast<UFireflyGameplayAbility>(Instance))
				{
					FireflyGA->OnAbilityInputCanceled(InputValue);
					FireflyGA->OnAbilityInputEvent(ETriggerEvent::Canceled, InputValue);
				}
			}
		}
	}
}

void UFireflyAbilitySystemComponent::AbilitySpecInputTriggered(FGameplayAbilitySpec& Spec,
	const FInputActionValue& InputValue)
{
	if (Spec.IsActive())
	{
		// The ability is active, so just pipe the input event to it
		if (Spec.Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced)
		{
			if (UFireflyGameplayAbility* FireflyGA = Cast<UFireflyGameplayAbility>(Spec.Ability))
			{
				FireflyGA->OnAbilityInputTriggered(InputValue);
				FireflyGA->OnAbilityInputEvent(ETriggerEvent::Triggered, InputValue);
			}
		}
		else
		{
			TArray<UGameplayAbility*> Instances = Spec.GetAbilityInstances();
			for (UGameplayAbility* Instance : Instances)
			{
				if (UFireflyGameplayAbility* FireflyGA = Cast<UFireflyGameplayAbility>(Instance))
				{
					FireflyGA->OnAbilityInputTriggered(InputValue);
					FireflyGA->OnAbilityInputEvent(ETriggerEvent::Triggered, InputValue);
				}
			}
		}
	}
}

void UFireflyAbilitySystemComponent::AbilitySpecInputCompleted(FGameplayAbilitySpec& Spec,
	const FInputActionValue& InputValue)
{
	if (Spec.IsActive())
	{
		// The ability is active, so just pipe the input event to it
		if (Spec.Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced)
		{
			if (UFireflyGameplayAbility* FireflyGA = Cast<UFireflyGameplayAbility>(Spec.Ability))
			{
				FireflyGA->OnAbilityInputCompleted(InputValue);
				FireflyGA->OnAbilityInputEvent(ETriggerEvent::Completed, InputValue);
			}
		}
		else
		{
			TArray<UGameplayAbility*> Instances = Spec.GetAbilityInstances();
			for (UGameplayAbility* Instance : Instances)
			{
				if (UFireflyGameplayAbility* FireflyGA = Cast<UFireflyGameplayAbility>(Instance))
				{
					FireflyGA->OnAbilityInputCompleted(InputValue);
					FireflyGA->OnAbilityInputEvent(ETriggerEvent::Completed, InputValue);
				}
			}
		}
	}
}

bool UFireflyAbilitySystemComponent::SetAbilityCooldownTimeRemaining_Internal(const UFireflyGameplayAbility* Ability,
	float NewTimeRemaining)
{
	if (!IsValid(Ability))
	{
		return false;
	}

	if (SetActiveGameplayEffectTimeRemainingByOwningTags(*Ability->GetCooldownTags(), NewTimeRemaining))
	{
		OnAbilityCooldownChanged.Broadcast(Ability->CurrentSpecHandle, NewTimeRemaining, Ability->GetCooldownDuration());

		return true;
	}

	return false;
}

bool UFireflyAbilitySystemComponent::SetAbilityCooldownTimeRemaining(const FGameplayAbilitySpecHandle& Handle,
	float NewTimeRemaining)
{
	const FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(Handle);
	if (!Spec)
	{
		return false;
	}

	return SetAbilityCooldownTimeRemaining_Internal(Cast<UFireflyGameplayAbility>(Spec->Ability), NewTimeRemaining);
}

bool UFireflyAbilitySystemComponent::ModifyAbilityCooldownTimeRemainingByAddition(
	const FGameplayAbilitySpecHandle& Handle, float AdditiveAmount)
{
	const FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(Handle);
	if (!Spec)
	{
		return false;
	}

	const UGameplayAbility* Ability = Spec->Ability;
	if (!IsValid(Ability))
	{
		return false;
	}

	const float NewTimeRemaining = Ability->GetCooldownTimeRemaining() + AdditiveAmount;

	return SetAbilityCooldownTimeRemaining_Internal(Cast<UFireflyGameplayAbility>(Ability), NewTimeRemaining);
}

bool UFireflyAbilitySystemComponent::ModifyAbilityCooldownTimeRemainingByMultiplier(
	const FGameplayAbilitySpecHandle& Handle, float Multiplier)
{
	const FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(Handle);
	if (!Spec)
	{
		return false;
	}

	const UGameplayAbility* Ability = Spec->Ability;
	if (!IsValid(Ability))
	{
		return false;
	}

	const float NewTimeRemaining = Ability->GetCooldownTimeRemaining() * Multiplier;

	return SetAbilityCooldownTimeRemaining_Internal(Cast<UFireflyGameplayAbility>(Ability), NewTimeRemaining);
}

void UFireflyAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle,
	UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(Handle, Ability);

	OnAbilityActivated.Broadcast(Handle);
}

void UFireflyAbilitySystemComponent::NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle,
	UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	Super::NotifyAbilityFailed(Handle, Ability, FailureReason);

	OnAbilityActivatingFailed.Broadcast(Handle, FailureReason);
}

void UFireflyAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability,
	bool bWasCancelled)
{
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);

	OnAbilityEnded.Broadcast(Handle, bWasCancelled);
}

const UFireflyAttributeSet* UFireflyAbilitySystemComponent::AddAttributeSet(TSubclassOf<UFireflyAttributeSet> AttributeSetClass)
{
	const UFireflyAttributeSet* AttributeSet = AddAttributeSetSubobject<UFireflyAttributeSet>(
		NewObject<UFireflyAttributeSet>(GetOwner(), AttributeSetClass));
	if (!IsValid(AttributeSet))
	{
		return nullptr;
	}

	RegisterGameplayAttributeDelegates(AttributeSet);

	return AttributeSet;
}

void UFireflyAbilitySystemComponent::RemoveAttributeSet(TSubclassOf<UFireflyAttributeSet> AttributeSetClass)
{
	UFireflyAttributeSet* AttributeSet = const_cast<UFireflyAttributeSet*>(GetSet<UFireflyAttributeSet>());

	ShutdownGameplayAttributeDelegates(AttributeSet);

	RemoveSpawnedAttribute(AttributeSet);
}

void UFireflyAbilitySystemComponent::InitAttributeSetFromCustomDataTable(
	TSubclassOf<UFireflyAttributeSet> AttributeSetClass, const UDataTable* DataTable, FName RowName,
	bool bCompletelyOverride)
{
	UFireflyAttributeSet* AttributeSet = const_cast<UFireflyAttributeSet*>(Cast<UFireflyAttributeSet>(GetAttributeSet(AttributeSetClass)));
	if (!IsValid(AttributeSet))
	{
		return;
	}

	AttributeSet->InitFromCustomDataTable(DataTable, RowName, bCompletelyOverride);
}

void UFireflyAbilitySystemComponent::K2_SetNumericAttributeBase(FGameplayAttribute Attribute, float NewBaseValue)
{
	SetNumericAttributeBase(Attribute, NewBaseValue);
}

void UFireflyAbilitySystemComponent::K2_ApplyModToAttribute(FGameplayAttribute Attribute,
	TEnumAsByte<EGameplayModOp::Type> ModifierOp, float ModifierMagnitude)
{
	ApplyModToAttribute(Attribute, ModifierOp, ModifierMagnitude);
}

void UFireflyAbilitySystemComponent::RegisterGameplayAttributeDelegates(const UFireflyAttributeSet* FireflyAttributeSet)
{
	if (!IsValid(FireflyAttributeSet))
	{
		return;
	}

	TArray<FGameplayAttribute> Attributes;
	GetAllAttributes(Attributes);

	for (const FGameplayAttribute Attribute : Attributes)
	{
		GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
	}
}

void UFireflyAbilitySystemComponent::ShutdownGameplayAttributeDelegates(const UFireflyAttributeSet* FireflyAttributeSet)
{
	if (!IsValid(FireflyAttributeSet))
	{
		return;
	}

	TArray<FGameplayAttribute> Attributes;
	GetAllAttributes(Attributes);

	for (const FGameplayAttribute Attribute : Attributes)
	{
		GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UFireflyAbilitySystemComponent::HandleOnGameplayAttributeValueChanged);
	}
}

void UFireflyAbilitySystemComponent::HandleOnGameplayAttributeValueChanged(const FOnAttributeChangeData& Data)
{
	if (Data.OldValue == Data.NewValue)
	{
		return;
	}

	const FGameplayEffectModCallbackData* ModData = Data.GEModData;
	FGameplayTagContainer SourceTags = FGameplayTagContainer();
	if (ModData)
	{
		SourceTags = *ModData->EffectSpec.CapturedSourceTags.GetAggregatedTags();
	}

	OnAttributeValueChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue, SourceTags);
}

bool UFireflyAbilitySystemComponent::SetActiveGameplayEffectTimeRemaining(FActiveGameplayEffect* Effect,
	float NewTimeRemaining)
{
	if (!Effect)
	{
		return false;
	}

	if (NewTimeRemaining > 0)
	{
		Effect->Spec.Duration = NewTimeRemaining;
	}
	else
	{
		Effect->Spec.Duration = 0.01f;
	}

	Effect->StartServerWorldTime = ActiveGameplayEffects.GetServerWorldTime();
	Effect->CachedStartServerWorldTime = Effect->StartServerWorldTime;
	Effect->StartWorldTime = ActiveGameplayEffects.GetWorldTime();
	ActiveGameplayEffects.MarkItemDirty(*Effect);
	ActiveGameplayEffects.CheckDuration(Effect->Handle);	

	Effect->EventSet.OnTimeChanged.Broadcast(Effect->Handle, Effect->StartWorldTime, Effect->GetDuration());
	OnGameplayEffectDurationChange(*Effect);

	return true;
}

bool UFireflyAbilitySystemComponent::SetActiveGameplayEffectTimeRemainingByHandle(const FActiveGameplayEffectHandle Handle,
	float NewTimeRemaining)
{
	if (!Handle.IsValid())
	{
		return false;
	}

	return SetActiveGameplayEffectTimeRemaining(ActiveGameplayEffects.GetActiveGameplayEffect(Handle), NewTimeRemaining);
}

bool UFireflyAbilitySystemComponent::SetActiveGameplayEffectTimeRemainingByAssetTags(
	const FGameplayTagContainer& AssetTags, float NewTimeRemaining)
{
	if (!AssetTags.IsValid())
	{
		return false;
	}

	for (const FActiveGameplayEffectHandle Handle : GetActiveEffects(FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(AssetTags)))
	{
		SetActiveGameplayEffectTimeRemainingByHandle(Handle, NewTimeRemaining);
	}

	return true;
}

bool UFireflyAbilitySystemComponent::SetActiveGameplayEffectTimeRemainingByOwningTags(
	const FGameplayTagContainer& OwningTags, float NewTimeRemaining)
{
	if (!OwningTags.IsValid())
	{
		return false;
	}

	for (const FActiveGameplayEffectHandle Handle : GetActiveEffects(FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(OwningTags)))
	{
		SetActiveGameplayEffectTimeRemainingByHandle(Handle, NewTimeRemaining);
	}

	return true;
}

void UFireflyAbilitySystemComponent::RegisterGameplayEffectDelegates()
{
	OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &UFireflyAbilitySystemComponent::HandleOnActiveGameplayEffectAdded);
	OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &UFireflyAbilitySystemComponent::HandleOnAnyGameplayEffectRemoved);
}

void UFireflyAbilitySystemComponent::ShutdownGameplayEffectDelegates()
{
	OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
	OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);
}

void UFireflyAbilitySystemComponent::HandleOnActiveGameplayEffectAdded(UAbilitySystemComponent* Target,
	const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	OnActiveGameplayEffectApplied.Broadcast(ActiveHandle);

	OnGameplayEffectStackChangeDelegate(ActiveHandle)->AddUObject(this, &UFireflyAbilitySystemComponent::HandleOnActiveGameplayEffectStackChanged);
	OnGameplayEffectTimeChangeDelegate(ActiveHandle)->AddUObject(this, &UFireflyAbilitySystemComponent::HandleOnActiveGameplayEffectTimeChanged);

	if (const UFireflyGameplayEffect* FireflyGE = Cast<UFireflyGameplayEffect>(SpecApplied.Def))
	{
		for (const auto CustomExec : FireflyGE->CustomExecutionClasses)
		{
			AddCustomExecution(CustomExec);
		}
	}
}

void UFireflyAbilitySystemComponent::HandleOnAnyGameplayEffectRemoved(const FActiveGameplayEffect& EffectRemoved)
{
	if (const UFireflyGameplayEffect* FireflyGE = Cast<UFireflyGameplayEffect>(EffectRemoved.Spec.Def))
	{
		for (const auto CustomExec : FireflyGE->CustomExecutionClasses)
		{
			RemoveCustomExecution(CustomExec);
		}
	}

	OnActiveGameplayEffectRemoved.Broadcast(EffectRemoved.Handle);
}

void UFireflyAbilitySystemComponent::HandleOnActiveGameplayEffectStackChanged(FActiveGameplayEffectHandle ActiveHandle,
	int32 NewStackCount, int32 PreviousStackCount)
{
	OnActiveGameplayEffectStackChanged.Broadcast(ActiveHandle, NewStackCount, PreviousStackCount);
}

void UFireflyAbilitySystemComponent::HandleOnActiveGameplayEffectTimeChanged(FActiveGameplayEffectHandle ActiveHandle,
	float NewStartTime, float NewDuration)
{
	OnActiveGameplayEffectTimeRemainingChanged.Broadcast(ActiveHandle, NewStartTime, NewDuration);
}

void UFireflyAbilitySystemComponent::RegisterGameplayTagDelegates()
{
	RegisterGenericGameplayTagEvent().AddUObject(this, &UFireflyAbilitySystemComponent::OnAnyGameplayTagStackChanged);
}

void UFireflyAbilitySystemComponent::ShutdownGameplayTagDelegates()
{
	RegisterGenericGameplayTagEvent().RemoveAll(this);
}

void UFireflyAbilitySystemComponent::OnAnyGameplayTagStackChanged(const FGameplayTag GameplayTag, const int32 NewCount)
{
	OnGameplayTagStackChanged.Broadcast(GameplayTag, NewCount);
}

void UFireflyAbilitySystemComponent::AddCustomExecution(
	TSubclassOf<UFireflyAbilitySystemCustomExecution> CustomExecution)
{
	CustomExecutionContainer->AddCustomExecution(CustomExecution);
}

void UFireflyAbilitySystemComponent::RemoveCustomExecution(
	TSubclassOf<UFireflyAbilitySystemCustomExecution> CustomExecution)
{
	CustomExecutionContainer->RemoveCustomExecution(CustomExecution);
}