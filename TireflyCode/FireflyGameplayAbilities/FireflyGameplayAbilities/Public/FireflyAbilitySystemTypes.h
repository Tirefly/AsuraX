// Copyright tzlFirefly, 2023. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"

#include "FireflyAbilitySystemTypes.generated.h"


class UFireflyGameplayEffect;


#pragma region GameplayEffect_UIData

// GameplayEffect的UI信息，提供了一个材质接口、一个贴图引用，和一个文本描述
// UI information for GameplayEffect, providing a material interface, a texture reference, and a text description.
USTRUCT(BlueprintType)
struct FFireflyGameplayEffectUIDisplay
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	UMaterialInterface* Material;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	UTexture2D* Texture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data, Meta = (MultiLine = "true"))
	FText Title;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data, Meta = (MultiLine = "true"))
	FText Description;

	FFireflyGameplayEffectUIDisplay()
	{
		Material = nullptr;
		Texture = nullptr;
		Title = FText();
		Description = FText();
	}

	bool operator==(const FFireflyGameplayEffectUIDisplay& Other) const
	{
		return Material == Other.Material
			&& Texture == Other.Texture
			&& Title.EqualTo(Other.Title)
			&& Description.EqualTo(Other.Description);
	}

	bool operator!=(const FFireflyGameplayEffectUIDisplay& Other) const
	{
		return Material != Other.Material
			|| Texture != Other.Texture
			|| !Title.EqualTo(Other.Title)
			|| !Description.EqualTo(Other.Description);
	}

	void operator=(const FFireflyGameplayEffectUIDisplay& Other);

	bool IsValid() const;

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);
};

#pragma endregion


#pragma region GameplayAbility_InputBinding

class UInputAction;

#define FIREFLY_TO_FLAG(EnumValue) (1 << static_cast<uint8>(EnumValue))

/**
 * 技能和输入绑定后，输入激活技能的策略。
 * The policy for activating the ability when it is bound to an input.
 */
UENUM(BlueprintType, meta = (Bitflags))
enum class EFireflyAbilityInputActivationPolicy : uint8
{
	// 该能力没有输入激活策略
	// This ability has no input activating policy
	None = 0,

	// 当输入开始时尝试激活技能。
	// Try to activate the ability when the input is started.
	OnInputStarted = 1,

	// 当输入触发时尝试激活技能。
	// Try to activate the ability when the input is triggered.
	OnInputTriggered = 2
};

/**
 * 技能和输入的绑定信息，包含技能规范句柄、输入事件句柄。
 * Input binding data for skills, including ability spec handles, and input event handles.
 */
USTRUCT()
struct FFireflyAbilityInputBindingData
{
	GENERATED_BODY()

public:
	// 输入绑定的所有技能的规范句柄
	// All abilities' spec handle bound to the input
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> Abilities;

	// 输入事件句柄：开始
	// Input event handle: Started
	UPROPERTY()
	uint32 HandleStarted = -1;

	// 输入事件句柄：进行中
	// Input event handle: Ongoing
	UPROPERTY()
	uint32 HandleOngoing = -1;

	// 输入事件句柄：取消
	// Input event handle: Canceled
	UPROPERTY()
	uint32 HandleCanceled = -1;

	// 输入事件句柄：触发
	// Input event handle: Triggered
	UPROPERTY()
	uint32 HandleTriggered = -1;

	// 输入事件句柄：完成
	// Input event handle: Completed
	UPROPERTY()
	uint32 HandleCompleted = -1;

	FFireflyAbilityInputBindingData() {}
};

#pragma endregion