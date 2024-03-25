// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "TireflyAttributeRangeSetting.generated.h"

// 定义属性范围类型的美剧
UENUM(BlueprintType)
enum class ETireflyAttributeRangeType : uint8
{
	None			UMETA(Tooptip = "无属性范围，表示没有特定的属性范围限制"),
	Numeric			UMETA(Tooptip = "数字属性范围，表示属性范围由具体的数字值定义"),
	Attribute		UMETA(Tooptip = "属性属性范围，表示属性范围由另一个属性定义，提供更复杂的属性依赖关系")
};

// 定义一个游戏性属性的数值范围设置的结构体
USTRUCT()
struct FTireflyAttributeRangeSetting
{
	GENERATED_BODY()

public:
	// 属性最小值的范围类型
	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute AttributeType = FGameplayAttribute();
	
	// 属性最小值的范围类型
	UPROPERTY(EditDefaultsOnly)
	ETireflyAttributeRangeType MinValueType = ETireflyAttributeRangeType::None;

	// 当最小值类型为数值时，该值作为属性的范围最小值
	UPROPERTY(EditDefaultsOnly, Meta = (EditCondition = "MinValueType == ETireflyAttributeRangeType::Numeric", EditConditionHides))
	float MinValue = 0.f;

	// 最小值类型为属性时，属性拥有者的指定类型属性值作为属性的范围最小值
	UPROPERTY(EditDefaultsOnly, Meta = (EditCondition = "MinValueType == ETireflyAttributeRangeType::Attribute", EditConditionHides))
	FGameplayAttribute MinAttribute = FGameplayAttribute();

	// 属性最大值的范围类型
	UPROPERTY(EditDefaultsOnly)
	ETireflyAttributeRangeType MaxValueType = ETireflyAttributeRangeType::None;

	// 当最大值类型为数值时，该值作为属性的范围最大值
	UPROPERTY(EditDefaultsOnly, Meta = (EditCondition = "MaxValueType == ETireflyAttributeRangeType::Numeric", EditConditionHides))
	float MaxValue = 0.f;

	// 最大值类型为属性时，属性拥有者的指定类型属性值作为属性的范围最大值
	UPROPERTY(EditDefaultsOnly, Meta = (EditCondition = "MaxValueType == ETireflyAttributeRangeType::Attribute", EditConditionHides))
	FGameplayAttribute MaxAttribute = FGameplayAttribute();

	FTireflyAttributeRangeSetting()
	{}

	FTireflyAttributeRangeSetting(const FGameplayAttribute& InAttribute)
		: AttributeType(InAttribute)
	{}

	friend uint32 GetTypeHash(const FTireflyAttributeRangeSetting& InSetting)
	{
		uint32 Hash = 0;
		Hash = HashCombine(Hash, GetTypeHash(InSetting.AttributeType));

		return Hash;
	}

	bool operator==(const FTireflyAttributeRangeSetting& Other) const
	{
		return AttributeType == Other.AttributeType;
	}
};