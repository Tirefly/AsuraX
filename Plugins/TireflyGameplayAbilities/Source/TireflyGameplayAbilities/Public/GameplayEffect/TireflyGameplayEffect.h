// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "TireflyGameplayEffect.generated.h"


/**
 * 
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
