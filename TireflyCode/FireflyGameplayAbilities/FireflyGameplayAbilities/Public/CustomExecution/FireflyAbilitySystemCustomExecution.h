// Copyright tzlFirefly, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FireflyAbilitySystemCustomExecution.generated.h"


struct FGameplayAttribute;
class UFireflyCustomExecution_PostAttributeBaseChange;
class UFireflyCustomExecution_PostAttributeChange;
class UFireflyCustomExecution_PostGameplayEffectExecute;
class UFireflyCustomExecution_PreAttributeBaseChange;
class UFireflyCustomExecution_PreAttributeChange;
class UFireflyCustomExecution_PreGameplayEffectExecute;


// 自定义执行逻辑的基类，会有很多基于此的派生类以类默认对象的形式在 GAS 执行的各个阶段被调用
// The base class for custom execution logic, from which many derived classes are created as class default objects and called at various stages of GAS execution.
UCLASS(Blueprintable)
class FIREFLYGAMEPLAYABILITIES_API UFireflyAbilitySystemCustomExecution : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override
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
};


UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyCustomExecutionContainer : public UObject
{
	GENERATED_BODY()

#pragma region Object

public:
	UFireflyCustomExecutionContainer(const FObjectInitializer& ObjectInitializer);

	virtual UWorld* GetWorld() const override;

protected:
	friend class UFireflyAbilitySystemComponent;

#pragma endregion


#pragma region CustomExecutionContainer

public:
	void AddCustomExecution(TSubclassOf<UFireflyAbilitySystemCustomExecution> CustomExecution);

	void RemoveCustomExecution(TSubclassOf<UFireflyAbilitySystemCustomExecution> CustomExecution);

protected:
	UPROPERTY()
	TArray<TSubclassOf<UFireflyCustomExecution_PreGameplayEffectExecute>> CustomExecutions_PreGameplayEffectExecute;

	UPROPERTY()
	TArray<TSubclassOf<UFireflyCustomExecution_PostGameplayEffectExecute>> CustomExecutions_PostGameplayEffectExecute;

	UPROPERTY()
	TArray<TSubclassOf<UFireflyCustomExecution_PreAttributeChange>> CustomExecutions_PreAttributeChange;

	UPROPERTY()
	TArray<TSubclassOf<UFireflyCustomExecution_PostAttributeChange>> CustomExecutions_PostAttributeChange;

	UPROPERTY()
	TArray<TSubclassOf<UFireflyCustomExecution_PreAttributeBaseChange>> CustomExecutions_PreAttributeBaseChange;

	UPROPERTY()
	TArray<TSubclassOf<UFireflyCustomExecution_PostAttributeBaseChange>> CustomExecutions_PostAttributeBaseChange;

#pragma endregion


#pragma region CustomExecution

public:
	virtual void HandleAttributeSetPreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data);

	virtual void HandleAttributeSetPostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data);

	virtual void HandleAttributeSetPreAttributeChange(UFireflyAbilitySystemComponent* FireflyASC
		, const FGameplayAttribute& Attribute, float& NewValue);

	virtual void HandleAttributeSetPostAttributeChange(UFireflyAbilitySystemComponent* FireflyASC
		, const FGameplayAttribute& Attribute, float OldValue, float NewValue);

	virtual void HandleAttributeSetPreAttributeBaseChange(UFireflyAbilitySystemComponent* FireflyASC
		, const FGameplayAttribute& Attribute, float& NewValue);

	virtual void HandleAttributeSetPostAttributeBaseChange(UFireflyAbilitySystemComponent* FireflyASC
		, const FGameplayAttribute& Attribute, float OldValue, float NewValue);

#pragma endregion
};