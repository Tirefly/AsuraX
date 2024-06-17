// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "TireflyGameplayEffect.generated.h"


// 属性与其对应的SetByCallerTag设置的结构体
USTRUCT()
struct FTireflySetByCallerTagConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute AttributeType = FGameplayAttribute();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SetByCallerTag = FGameplayTag();
};


/** 方便开发者编辑操作的GameplayEffect，同时可以在编辑器中创建实例 */
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

#pragma region GameplayEffect

public:
	UTireflyGameplayEffect(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma endregion


#pragma region GameplayEffect_SetByCaller

protected:
	bool ProcessDurationSetByCaller();
	
	bool ProcessModiferSetByCaller();

#pragma endregion
};
