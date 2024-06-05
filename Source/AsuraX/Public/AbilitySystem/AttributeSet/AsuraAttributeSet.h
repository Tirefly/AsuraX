// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAttribute/TireflyAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AsuraAttributeSet.generated.h"


// 所有角色共用的基础属性集
UCLASS()
class ASURAX_API UAsuraAttributeSet : public UTireflyAttributeSet
{
	GENERATED_BODY()

#pragma region AttributeSet

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion
	

#pragma region AttributeDerivation_Strength

public:
	// 基础属性：力量
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Base Attribute")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Strength);

	// 派生属性：攻击力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Attack, Category = "Derived Attribute")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Attack);

	// 战斗属性：暴击倍率
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalMultiplier, Category = "Derived Attribute")
	FGameplayAttributeData CriticalMultiplier;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, CriticalMultiplier);

protected:
	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldStrength)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Strength, OldStrength); }

	UFUNCTION()
	virtual void OnRep_Attack(const FGameplayAttributeData& OldAttack)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Attack, OldAttack); }

	UFUNCTION()
	virtual void OnRep_CriticalMultiplier(const FGameplayAttributeData& OldCriticalMultiplier)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, CriticalMultiplier, OldCriticalMultiplier) }

#pragma endregion


#pragma region AttributeDerivation_Physique

public:
	// 基础属性：体魄
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Physique, Category = "Base Attribute")
	FGameplayAttributeData Physique;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Physique);

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

protected:
	UFUNCTION()
	virtual void OnRep_Physique(const FGameplayAttributeData& OldPhysique)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Physique, OldPhysique); }

	UFUNCTION()
	virtual void OnRep_Defense(const FGameplayAttributeData& OldDefense)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Defense, OldDefense);	}
	
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, MaxHealth, OldMaxHealth);	}

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Health, OldHealth); }

#pragma endregion


#pragma region AttributeDerivation_Agility

public:
	// 基础属性：敏捷
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "Base Attribute")
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Agility);

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
	virtual void OnRep_Agility(const FGameplayAttributeData& OldAgility)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Agility, OldAgility);	}

	UFUNCTION()
	virtual void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, AttackSpeed, OldAttackSpeed);	}

	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, MoveSpeed, OldMoveSpeed);	}

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

	// 战斗属性：暴击抵抗
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalResistance, Category = "Combat Attribute")
	FGameplayAttributeData CriticalResistance;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, CriticalResistance);

	// 战斗属性：伤害加成
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DamageBonus, Category = "Combat Attribute")
	FGameplayAttributeData DamageBonus;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, DamageBonus);

	// 战斗属性：伤害减免
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DamageReduce, Category = "Combat Attribute")
	FGameplayAttributeData DamageReduce;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, DamageReduce);

	// 战斗属性：伤害
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Damage, Category = "Combat Attribute")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Damage);

protected:
	UFUNCTION()
	virtual void OnRep_HitRate(const FGameplayAttributeData& OldHitRate)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, HitRate, OldHitRate);	}

	UFUNCTION()
	virtual void OnRep_DodgeRate(const FGameplayAttributeData& OldDodgeRate)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, DodgeRate, OldDodgeRate);	}

	UFUNCTION()
	virtual void OnRep_CriticalRate(const FGameplayAttributeData& OldCriticalRate)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, CriticalRate, OldCriticalRate); }

	UFUNCTION()
	virtual void OnRep_CriticalResistance(const FGameplayAttributeData& OldCriticalResistance)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, CriticalResistance, OldCriticalResistance); }

	UFUNCTION()
	virtual void OnRep_DamageBonus(const FGameplayAttributeData& OldDamageBonus)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, DamageBonus, OldDamageBonus); }

	UFUNCTION()
	virtual void OnRep_DamageReduce(const FGameplayAttributeData& OldDamageReduce)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, DamageReduce, OldDamageReduce); }

	UFUNCTION()
	virtual void OnRep_Damage(const FGameplayAttributeData& OldDamage)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Damage, OldDamage); }

#pragma endregion


#pragma region TheFiveElement

public:
	// 五行属性：金
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Metal, Category = "The Five Element")
	FGameplayAttributeData Metal;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Metal);

	// 五行属性：木
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Wood, Category = "The Five Element")
	FGameplayAttributeData Wood;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Wood);

	// 五行属性：水
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Water, Category = "The Five Element")
	FGameplayAttributeData Water;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Water);

	// 五行属性：火
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Fire, Category = "The Five Element")
	FGameplayAttributeData Fire;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Fire);

	// 五行属性：土
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Earth, Category = "The Five Element")
	FGameplayAttributeData Earth;
	ATTRIBUTE_ACCESSORS(UAsuraAttributeSet, Earth);

protected:
	UFUNCTION()
	virtual void OnRep_Metal(const FGameplayAttributeData& OldMetal)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Metal, OldMetal);}

	UFUNCTION()
	virtual void OnRep_Wood(const FGameplayAttributeData& OldWood)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Wood, OldWood);}

	UFUNCTION()
	virtual void OnRep_Water(const FGameplayAttributeData& OldWater)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Water, OldWater);}

	UFUNCTION()
	virtual void OnRep_Fire(const FGameplayAttributeData& OldFire)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Fire, OldFire);}

	UFUNCTION()
	virtual void OnRep_Earth(const FGameplayAttributeData& OldEarth)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraAttributeSet, Earth, OldEarth);}

#pragma endregion
};
