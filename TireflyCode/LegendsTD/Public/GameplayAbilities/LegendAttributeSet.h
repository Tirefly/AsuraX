// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAttribute/FireflyAttributeSet.h"
#include "LegendAttributeSet.generated.h"


UCLASS()
class LEGENDSTD_API ULegendAttributeSet : public UFireflyAttributeSet
{
	GENERATED_BODY()

#pragma region AttributeSet

public:
	ULegendAttributeSet(const FObjectInitializer& ObjectInitializer);

#pragma endregion


#pragma region BaseAttribtue

public:
	// 体魄
	UPROPERTY(BlueprintReadOnly, Category = "Base Attribute")
	FGameplayAttributeData Physique;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, Physique);

	// 身法
	UPROPERTY(BlueprintReadOnly, Category = "Base Attribute")
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, Agility);

	// 灵智
	UPROPERTY(BlueprintReadOnly, Category = "Base Attribute")
	FGameplayAttributeData Wisdom;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, Wisdom);

#pragma endregion


#pragma region DerivedAttribtue

public:
	// 最大生命值
	UPROPERTY(BlueprintReadOnly, Category = "Derived Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, MaxHealth);

	// 当前生命值
	UPROPERTY(BlueprintReadOnly, Category = "Derived Attribute")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, CurrentHealth);

	// 生命值恢复速度
	UPROPERTY(BlueprintReadOnly, Category = "Derived Attribute")
	FGameplayAttributeData HealthRecoveryRate;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, HealthRecoveryRate);

	// 防御力
	UPROPERTY(BlueprintReadOnly, Category = "Derived Attribute")
	FGameplayAttributeData DefensiveForce;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, DefensiveForce);

	// 攻击速度
	UPROPERTY(BlueprintReadOnly, Category = "Derived Attribute")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, AttackSpeed);

	// 攻击恢复法力量
	UPROPERTY(BlueprintReadOnly, Category = "Derived Attribute")
	FGameplayAttributeData ManaRecoveryOfAttack;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, ManaRecoveryOfAttack);

	// 技能强度
	UPROPERTY(BlueprintReadOnly, Category = "Derived Attribute")
	FGameplayAttributeData SkillIntensity;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, SkillIntensity);

	// 攻击力
	UPROPERTY(BlueprintReadOnly, Category = "Derived Attribute")
	FGameplayAttributeData AttackForce;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, AttackForce);

#pragma endregion


#pragma region AdditiveAttribute

public:
	// 生命偷取
	UPROPERTY(BlueprintReadOnly, Category = "Additive Attribute")
	FGameplayAttributeData HealthStealing;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, HealthStealing);

	// 法力值恢复速度
	UPROPERTY(BlueprintReadOnly, Category = "Additive Attribute")
	FGameplayAttributeData ManaRecoveryRate;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, ManaRecoveryRate);

	// 护盾值
	UPROPERTY(BlueprintReadOnly, Category = "Additive Attribute")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, Shield);

	// 暴击率
	UPROPERTY(BlueprintReadOnly, Category = "Additive Attribute")
	FGameplayAttributeData CriticalRate;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, CriticalRate);

	// 暴击伤害加成
	UPROPERTY(BlueprintReadOnly, Category = "Additive Attribute")
	FGameplayAttributeData CriticalEnhanceRate;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, CriticalEnhanceRate);

	// 伤害加成
	UPROPERTY(BlueprintReadOnly, Category = "Additive Attribute")
	FGameplayAttributeData DamageBonus;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, DamageBonus);

	// 伤害减免
	UPROPERTY(BlueprintReadOnly, Category = "Additive Attribute")
	FGameplayAttributeData DamageReduction;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, DamageReduction);

	// 闪避率
	UPROPERTY(BlueprintReadOnly, Category = "Additive Attribute")
	FGameplayAttributeData DodgeRate;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, DodgeRate);

	// 受到的伤害
	UPROPERTY(BlueprintReadOnly, Category = "Additive Attribute")
	FGameplayAttributeData DamageSuffered;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, DamageSuffered);

#pragma endregion


#pragma region ConstantAttribute

public:
	// 体型
	UPROPERTY(BlueprintReadOnly, Category = "Constant Attribute")
	FGameplayAttributeData Size;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, Size);

	// 攻击范围
	UPROPERTY(BlueprintReadOnly, Category = "Constant Attribute")
	FGameplayAttributeData AttackRange;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, AttackRange);

	// 攻击目标数
	UPROPERTY(BlueprintReadOnly, Category = "Constant Attribute")
	FGameplayAttributeData AttackTargetSum;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, AttackTargetSum);

	// 最大法力值
	UPROPERTY(BlueprintReadOnly, Category = "Constant Attribute")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, MaxMana);

	// 当前法力值
	UPROPERTY(BlueprintReadOnly, Category = "Constant Attribute")
	FGameplayAttributeData CurrentMana;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, CurrentMana);

	// 移动速度
	UPROPERTY(BlueprintReadOnly, Category = "Constant Attribute")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(ULegendAttributeSet, MovementSpeed);

#pragma endregion


#pragma region PoseGameplayEffectExecute

protected:
	static void PostDamageExecute(const struct FGameplayEffectModCallbackData& Data);

public:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

#pragma endregion


#pragma region PreAttributeChange

protected:
	void ClampAttributeValue(const FGameplayAttribute& Attribute, float& NewValue) const;

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

#pragma endregion


#pragma region PostAttributeChange

protected:
	void AdjustAttributeForMaxChange(const FGameplayAttribute& CurrentAttribute
		, const FGameplayAttribute& MaxAttribute, float OldValue, float NewValue) const;

public:
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

#pragma endregion
};
