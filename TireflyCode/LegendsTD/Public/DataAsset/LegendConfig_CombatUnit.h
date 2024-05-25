// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayEffectTypes.h"
#include "LegendConfig_CombatUnit.generated.h"


enum class EHeroRarity : uint8;
class UDataTable;
class UGameplayEffect;
class UGameplayAbility;
class UFireflyAbilitySystemCustomExecution;


// 定义属性范围类型的美剧
UENUM(BlueprintType)
enum class EFireflyAttributeRangeType : uint8
{
	None			UMETA(Tooptip = "无属性范围，表示没有特定的属性范围限制"),
	Numeric			UMETA(Tooptip = "数字属性范围，表示属性范围由具体的数字值定义"),
	Attribute		UMETA(Tooptip = "属性属性范围，表示属性范围由另一个属性定义，提供更复杂的属性依赖关系")
};


// 定义一个游戏性属性的数值范围设置的结构体
USTRUCT(BlueprintType)
struct LEGENDSTD_API FFireflyAttributeRange
{
	GENERATED_BODY()

public:
	// 属性最小值的范围类型
	UPROPERTY(EditDefaultsOnly)
	EFireflyAttributeRangeType MinValueType = EFireflyAttributeRangeType::None;

	// 当最小值类型为数值时，该值作为属性的范围最小值
	UPROPERTY(EditDefaultsOnly, Meta = (EditCondition = "MinValueType == EFireflyAttributeRangeType::Numeric", EditConditionHides))
	float MinValue = 0.f;

	// 最小值类型为属性时，属性拥有者的指定类型属性值作为属性的范围最小值
	UPROPERTY(EditDefaultsOnly, Meta = (EditCondition = "MinValueType == EFireflyAttributeRangeType::Attribute", EditConditionHides))
	FGameplayAttribute MinAttribute = FGameplayAttribute();

	// 属性最大值的范围类型
	UPROPERTY(EditDefaultsOnly)
	EFireflyAttributeRangeType MaxValueType = EFireflyAttributeRangeType::None;

	// 当最大值类型为数值时，该值作为属性的范围最大值
	UPROPERTY(EditDefaultsOnly, Meta = (EditCondition = "MaxValueType == EFireflyAttributeRangeType::Numeric", EditConditionHides))
	float MaxValue = 0.f;

	// 最大值类型为属性时，属性拥有者的指定类型属性值作为属性的范围最大值
	UPROPERTY(EditDefaultsOnly, Meta = (EditCondition = "MaxValueType == EFireflyAttributeRangeType::Attribute", EditConditionHides))
	FGameplayAttribute MaxAttribute = FGameplayAttribute();
};


// 战斗单位在游戏中的默认通用配置
UCLASS(BlueprintType)
class LEGENDSTD_API ULegendConfig_CombatUnit : public UPrimaryDataAsset
{
	GENERATED_BODY()

#pragma region PrimaryDataAsset

public:
	ULegendConfig_CombatUnit(const FObjectInitializer& ObjectInitializer);

#pragma endregion


#pragma region GameplayAbilitySystem

public:
	// 战斗单位属性数值表
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayAbilitySystem)
	TMap<int32, UDataTable*> CombatUnitAttributeTable;

	// 战斗单位初始的能力
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayAbilitySystem)
	TSet<TSubclassOf<UGameplayAbility>> CombatUnitInitialAbilities;

	// 战斗单位初始的效果
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayAbilitySystem)
	TSet<TSubclassOf<UGameplayEffect>> CombatUnitInitialEffects;

	// 战斗单位初始的自定义执行
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayAbilitySystem)
	TSet<TSubclassOf<UFireflyAbilitySystemCustomExecution>> CombatUnitInitialCustomExecutions;

#pragma endregion


#pragma region CombatUnitConfig

public:
	bool GetAttributeRange(const FGameplayAttribute& Attribute, class UAbilitySystemComponent* ASC,
		float& RangeMin, float& RangeMax) const;

public:
	// 战斗单位部分属性的初始值
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CombatUnitConfig)
	TMap<FGameplayAttribute, float> AttributeInitializer;

	// 战斗单位部分属性的值范围
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CombatUnitConfig)
	TMap<FGameplayAttribute, FFireflyAttributeRange> AttributeRangeSetting;

	// 战斗单位派生属性的参数值
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CombatUnitConfig)
	TMap<FGameplayAttribute, float> DerivedAttributeCoefficients;

	// 攻击速度计算参数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CombatUnitConfig)
	FVector2D AttackSpeedCoefficients;

#pragma endregion


#pragma region SynergyConfig

public:
	// 羁绊激活后，战斗单位增强属性的效果
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SynergyConfig)
	TSubclassOf<UGameplayEffect> AttributeEnhancingSynergyEffect;

	// 羁绊激活后，战斗单位增强的属性值（依据羁绊内的英雄稀有度）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SynergyConfig)
	TMap<EHeroRarity, int32> AttributeEnhancingSynergyMagnitude;

#pragma endregion
};
