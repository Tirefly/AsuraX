// Copyright tzlFirefly, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectUIData.h"
#include "FireflyAbilitySystemTypes.h"
#include "FireflyGameplayEffectUIData.generated.h"


// 基于 GameplayEffectUIData 扩展的游戏性效果UI信息类，提供了一个包含材质接口、贴图引用，和文本描述的结构体
// A game effect UI information class based on GameplayEffectUIData, providing a structure that includes a material interface, a texture reference, and a text description.
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyGameplayEffectUIData : public UGameplayEffectUIData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data, Meta = (MultiLine = "true"))
	FFireflyGameplayEffectUIDisplay UIData;
};