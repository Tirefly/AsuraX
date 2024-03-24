// Copyright Tirefly. All Rights Reserved.


#include "TireflyAbilitySystemComponent.h"

#include "AbilitySystemLog.h"
#include "GameplayEffectExtension.h"
#include "GameplayAttribute/TireflyAttributeSet.h"

UTireflyAbilitySystemComponent::UTireflyAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTireflyAbilitySystemComponent::K2_InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		UE_LOG(LogAbilitySystem, Warning, TEXT("UTireflyAbilitySystemComponent::K2_InitAbilityActorInfo: Can't call this from a simulated proxy actor."));		return;
		return;
	}
	
	InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}

const UTireflyAttributeSet* UTireflyAbilitySystemComponent::AddAttributeSet(
	TSubclassOf<UTireflyAttributeSet> AttributeSetClass)
{
	const UTireflyAttributeSet* AttributeSet = AddAttributeSetSubobject<UTireflyAttributeSet>(
		NewObject<UTireflyAttributeSet>(GetOwner(), AttributeSetClass));
	if (!IsValid(AttributeSet))
	{
		return nullptr;
	}

	RegisterGameplayAttributeDelegates(AttributeSet);

	return AttributeSet;
}

void UTireflyAbilitySystemComponent::RemoveAttributeSet(TSubclassOf<UTireflyAttributeSet> AttributeSetClass)
{
	UTireflyAttributeSet* AttributeSet = const_cast<UTireflyAttributeSet*>(GetSet<UTireflyAttributeSet>());

	ShutdownGameplayAttributeDelegates(AttributeSet);

	RemoveSpawnedAttribute(AttributeSet);
}

void UTireflyAbilitySystemComponent::InitAttributeSetFromDataTableRow(
	TSubclassOf<UTireflyAttributeSet> AttributeSetClass, const UDataTable* DataTable, FName RowName,
	bool bIgnoreGameplayEffect)
{
	UTireflyAttributeSet* AttributeSet = const_cast<UTireflyAttributeSet*>(Cast<UTireflyAttributeSet>(GetAttributeSet(AttributeSetClass)));
	if (!IsValid(AttributeSet))
	{
		return;
	}

	AttributeSet->InitFromDataTableRow(DataTable, RowName, bIgnoreGameplayEffect);
}

void UTireflyAbilitySystemComponent::K2_SetNumericAttributeBase(FGameplayAttribute Attribute, float NewBaseValue)
{
	SetNumericAttributeBase(Attribute, NewBaseValue);
}

void UTireflyAbilitySystemComponent::K2_ApplyModToAttribute(FGameplayAttribute Attribute,
	TEnumAsByte<EGameplayModOp::Type> ModifierOp, float ModifierMagnitude)
{
	ApplyModToAttribute(Attribute, ModifierOp, ModifierMagnitude);
}

void UTireflyAbilitySystemComponent::RegisterGameplayAttributeDelegates(const UTireflyAttributeSet* TireflyAttributeSet)
{
	if (!IsValid(TireflyAttributeSet))
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

void UTireflyAbilitySystemComponent::ShutdownGameplayAttributeDelegates(const UTireflyAttributeSet* TireflyAttributeSet)
{
	if (!IsValid(TireflyAttributeSet))
	{
		return;
	}

	TArray<FGameplayAttribute> Attributes;
	GetAllAttributes(Attributes);

	for (const FGameplayAttribute Attribute : Attributes)
	{
		GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this
			, &UTireflyAbilitySystemComponent::HandleOnGameplayAttributeValueChanged);
	}
}

void UTireflyAbilitySystemComponent::HandleOnGameplayAttributeValueChanged(const FOnAttributeChangeData& Data)
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
