// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAttribute/TireflyAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AsuraAttributeSet.generated.h"



UCLASS()
class ASURAX_API UAsuraAttributeSet : public UTireflyAttributeSet
{
	GENERATED_BODY()

#pragma region AttributeSet

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion
	

#pragma region BaseAttribute

public:
	// 基础属性：力量
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Base Attribute")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Strength);

	// 基础属性：体魄
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Physique, Category = "Base Attribute")
	FGameplayAttributeData Physique;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Physique);

	// 基础属性：敏捷
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "Base Attribute")
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Agility);

protected:
	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldStrength)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Strength, OldStrength);
	}

	UFUNCTION()
	virtual void OnRep_Physique(const FGameplayAttributeData& OldPhysique)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Physique, OldPhysique);
	}

	UFUNCTION()
	virtual void OnRep_Agility(const FGameplayAttributeData& OldAgility)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Agility, OldAgility);
	}

#pragma endregion


#pragma region DerivedAttribute

public:
	// 派生属性：攻击力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Attack, Category = "Derived Attribute")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Attack);

	// 派生属性：防御力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Defense, Category = "Derived Attribute")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Defense);

	// 派生属性：最大生命值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Derived Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, MaxHealth);

	// 派生属性：生命值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Derived Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Health);

	// 派生属性：攻击速度
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Derived Attribute")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, AttackSpeed);

	// 派生属性：移动速度
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeed, Category = "Derived Attribute")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, MoveSpeed);

protected:
	UFUNCTION()
	virtual void OnRep_Attack(const FGameplayAttributeData& OldAttack)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Attack, OldAttack);
	}

	UFUNCTION()
	virtual void OnRep_Defense(const FGameplayAttributeData& OldDefense)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Defense, OldDefense);
	}
	
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, MaxHealth, OldMaxHealth);
	}

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Health, OldHealth);
	}

	UFUNCTION()
	virtual void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, AttackSpeed, OldAttackSpeed);
	}

	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, MoveSpeed, OldMoveSpeed);
	}

#pragma endregion


#pragma region CombatAttribute

public:
	// 战斗属性：命中率
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HitRate, Category = "Combat Attribute")
	FGameplayAttributeData HitRate;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, HitRate);

	// 战斗属性：闪避率
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DodgeRate, Category = "Combat Attribute")
	FGameplayAttributeData DodgeRate;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, DodgeRate);

	// 战斗属性：暴击率
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalRate, Category = "Combat Attribute")
	FGameplayAttributeData CriticalRate;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, CriticalRate);

	// 战斗属性：暴击倍率
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalMultiplier, Category = "Combat Attribute")
	FGameplayAttributeData CriticalMultiplier;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, CriticalMultiplier);

	// 战斗属性：暴击抵抗
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalResistance, Category = "Combat Attribute")
	FGameplayAttributeData CriticalResistance;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, CriticalResistance);

protected:
	UFUNCTION()
	virtual void OnRep_HitRate(const FGameplayAttributeData& OldHitRate)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, HitRate, OldHitRate);
	}

	UFUNCTION()
	virtual void OnRep_DodgeRate(const FGameplayAttributeData& OldDodgeRate)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, DodgeRate, OldDodgeRate);
	}

	UFUNCTION()
	virtual void OnRep_CriticalRate(const FGameplayAttributeData& OldCriticalRate)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, CriticalRate, OldCriticalRate);
	}

	UFUNCTION()
	virtual void OnRep_CriticalMultiplier(const FGameplayAttributeData& OldCriticalMultiplier)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, CriticalMultiplier, OldCriticalMultiplier)
	}

	UFUNCTION()
	virtual void OnRep_CriticalResistance(const FGameplayAttributeData& OldCriticalResistance)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, CriticalResistance, OldCriticalResistance);
	}

#pragma endregion
};
