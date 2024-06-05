// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AsuraAttributeSet.h"
#include "AsuraPlayerAttributeSet.generated.h"


// 所有玩家角色共用的基础属性集
UCLASS()
class ASURAX_API UAsuraPlayerAttributeSet : public UAsuraAttributeSet
{
	GENERATED_BODY()

#pragma region AttributeSet

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion


#pragma region PlayerAttribute

public:
	// 玩家属性：最大气势值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxSprite, Category = "Player Attribute")
	FGameplayAttributeData MaxSprite;
	ATTRIBUTE_ACCESSORS(UAsuraPlayerAttributeSet, MaxSprite);

	// 玩家属性：气势值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Sprite, Category = "Player Attribute")
	FGameplayAttributeData Sprite;
	ATTRIBUTE_ACCESSORS(UAsuraPlayerAttributeSet, Sprite);
	
	// 玩家属性：气势获得
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SpriteGain, Category = "Player Attribute")
	FGameplayAttributeData SpriteGain;
	ATTRIBUTE_ACCESSORS(UAsuraPlayerAttributeSet, SpriteGain);

	// 玩家属性：气势流失
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SpriteLoss, Category = "Player Attribute")
	FGameplayAttributeData SpriteLoss;
	ATTRIBUTE_ACCESSORS(UAsuraPlayerAttributeSet, SpriteLoss);

protected:
	UFUNCTION()
	virtual void OnRep_MaxSprite(const FGameplayAttributeData& OldMaxSprite)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraPlayerAttributeSet, MaxSprite, OldMaxSprite); }
	
	UFUNCTION()
	virtual void OnRep_Sprite(const FGameplayAttributeData& OldSprite)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraPlayerAttributeSet, Sprite, OldSprite); }
	
	UFUNCTION()
	virtual void OnRep_SpriteGain(const FGameplayAttributeData& OldSpriteGain)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraPlayerAttributeSet, SpriteGain, OldSpriteGain); }

	UFUNCTION()
	virtual void OnRep_SpriteLoss(const FGameplayAttributeData& OldSpriteLoss)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraPlayerAttributeSet, SpriteLoss, OldSpriteLoss); }

#pragma endregion
};
