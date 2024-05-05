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


/**
 * 扩展了一些功能支持的GameplayEffect，扩展的功能如下：
 */
UCLASS()
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

#pragma region GameplayEffect

public:
	UTireflyGameplayEffect(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	/** UObject interface */
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma endregion


#pragma region GameplayEffect_SetByCaller

protected:
	void ProcessDurationSetByCaller();
	
	void ProcessModiferSetByCaller();

#pragma endregion
};
