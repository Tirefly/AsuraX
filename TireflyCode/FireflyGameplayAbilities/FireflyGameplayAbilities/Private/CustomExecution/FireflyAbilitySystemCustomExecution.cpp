// Copyright tzlFirefly, 2023. All Rights Reserved.


#include "CustomExecution/FireflyAbilitySystemCustomExecution.h"

#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "CustomExecution/FireflyCustomExecution_PostAttributeBaseChange.h"
#include "CustomExecution/FireflyCustomExecution_PostAttributeChange.h"
#include "CustomExecution/FireflyCustomExecution_PostGameplayEffectExecute.h"
#include "CustomExecution/FireflyCustomExecution_PreAttributeBaseChange.h"
#include "CustomExecution/FireflyCustomExecution_PreAttributeChange.h"
#include "CustomExecution/FireflyCustomExecution_PreGameplayEffectExecute.h"


UFireflyCustomExecutionContainer::UFireflyCustomExecutionContainer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UWorld* UFireflyCustomExecutionContainer::GetWorld() const
{
	if (!HasAnyFlags(RF_ClassDefaultObject) && !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		if (const AActor* Outer = GetTypedOuter<AActor>())
		{
			return Outer->GetWorld();
		}
	}

	return nullptr;
}

void UFireflyCustomExecutionContainer::AddCustomExecution(
	TSubclassOf<UFireflyAbilitySystemCustomExecution> CustomExecution)
{
	if (CustomExecution->IsChildOf(UFireflyCustomExecution_PreGameplayEffectExecute::StaticClass()))
	{
		CustomExecutions_PreGameplayEffectExecute.Emplace(CustomExecution);
	}
	else if (CustomExecution->IsChildOf(UFireflyCustomExecution_PostGameplayEffectExecute::StaticClass()))
	{
		CustomExecutions_PostGameplayEffectExecute.Emplace(CustomExecution);
	}
	else if (CustomExecution->IsChildOf(UFireflyCustomExecution_PreAttributeChange::StaticClass()))
	{
		CustomExecutions_PreAttributeChange.Emplace(CustomExecution);
	}
	else if (CustomExecution->IsChildOf(UFireflyCustomExecution_PostAttributeChange::StaticClass()))
	{
		CustomExecutions_PostAttributeChange.Emplace(CustomExecution);
	}
	else if (CustomExecution->IsChildOf(UFireflyCustomExecution_PreAttributeBaseChange::StaticClass()))
	{
		CustomExecutions_PreAttributeBaseChange.Emplace(CustomExecution);
	}
	else if (CustomExecution->IsChildOf(UFireflyCustomExecution_PostAttributeBaseChange::StaticClass()))
	{
		CustomExecutions_PostAttributeBaseChange.Emplace(CustomExecution);
	}
}

void UFireflyCustomExecutionContainer::RemoveCustomExecution(
	TSubclassOf<UFireflyAbilitySystemCustomExecution> CustomExecution)
{
	if (CustomExecution->IsChildOf(UFireflyCustomExecution_PreGameplayEffectExecute::StaticClass()))
	{
		CustomExecutions_PreGameplayEffectExecute.RemoveSingleSwap(CustomExecution.Get());
	}
	else if (CustomExecution->IsChildOf(UFireflyCustomExecution_PostGameplayEffectExecute::StaticClass()))
	{
		CustomExecutions_PostGameplayEffectExecute.RemoveSingleSwap(CustomExecution.Get());
	}
	else if (CustomExecution->IsChildOf(UFireflyCustomExecution_PreAttributeChange::StaticClass()))
	{
		CustomExecutions_PreAttributeChange.RemoveSingleSwap(CustomExecution.Get());
	}
	else if (CustomExecution->IsChildOf(UFireflyCustomExecution_PostAttributeChange::StaticClass()))
	{
		CustomExecutions_PostAttributeChange.RemoveSingleSwap(CustomExecution.Get());
	}
	else if (CustomExecution->IsChildOf(UFireflyCustomExecution_PreAttributeBaseChange::StaticClass()))
	{
		CustomExecutions_PreAttributeBaseChange.RemoveSingleSwap(CustomExecution.Get());
	}
	else if (CustomExecution->IsChildOf(UFireflyCustomExecution_PostAttributeBaseChange::StaticClass()))
	{
		CustomExecutions_PostAttributeBaseChange.RemoveSingleSwap(CustomExecution.Get());
	}
}

void UFireflyCustomExecutionContainer::HandleAttributeSetPreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	for (auto CustomExecution : CustomExecutions_PreGameplayEffectExecute)
	{
		CustomExecution.GetDefaultObject()->HandlePreGameplayEffectExecute(Data.EffectSpec.GetContext().GetInstigatorAbilitySystemComponent()
			, &Data.Target, Data.EffectSpec, Data.EvaluatedData, Data.EvaluatedData);
	}
}

void UFireflyCustomExecutionContainer::HandleAttributeSetPostGameplayEffectExecute(
	const FGameplayEffectModCallbackData& Data)
{
	for (auto CustomExecution : CustomExecutions_PostGameplayEffectExecute)
	{
		CustomExecution.GetDefaultObject()->HandlePostGameplayEffectExecute(Data.EffectSpec.GetContext().GetInstigatorAbilitySystemComponent()
			, &Data.Target, Data.EffectSpec, Data.EvaluatedData);
	}
}

void UFireflyCustomExecutionContainer::HandleAttributeSetPreAttributeChange(UFireflyAbilitySystemComponent* FireflyASC,
	const FGameplayAttribute& Attribute, float& NewValue)
{
	for (auto CustomExecution : CustomExecutions_PreAttributeChange)
	{
		CustomExecution.GetDefaultObject()->HandlePreAttributeChange(FireflyASC, Attribute, NewValue, NewValue);
	}
}

void UFireflyCustomExecutionContainer::HandleAttributeSetPostAttributeChange(UFireflyAbilitySystemComponent* FireflyASC,
	const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	for (auto CustomExecution : CustomExecutions_PostAttributeChange)
	{
		CustomExecution.GetDefaultObject()->HandlePostAttributeChange(FireflyASC, Attribute, OldValue, NewValue);
	}
}

void UFireflyCustomExecutionContainer::HandleAttributeSetPreAttributeBaseChange(
	UFireflyAbilitySystemComponent* FireflyASC, const FGameplayAttribute& Attribute, float& NewValue)
{
	for (auto CustomExecution : CustomExecutions_PreAttributeBaseChange)
	{
		CustomExecution.GetDefaultObject()->HandlePreAttributeBaseChange(FireflyASC, Attribute, NewValue, NewValue);
	}
}

void UFireflyCustomExecutionContainer::HandleAttributeSetPostAttributeBaseChange(
	UFireflyAbilitySystemComponent* FireflyASC, const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	for (auto CustomExecution : CustomExecutions_PostAttributeBaseChange)
	{
		CustomExecution.GetDefaultObject()->HandlePostAttributeBaseChange(FireflyASC, Attribute, OldValue, NewValue);
	}
}