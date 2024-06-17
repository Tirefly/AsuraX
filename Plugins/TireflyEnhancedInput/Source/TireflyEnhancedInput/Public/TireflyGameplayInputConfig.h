// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TireflyGameplayInputConfig.generated.h"


class UInputAction;
class UInputMappingContext;


UENUM(BlueprintType)
enum class ETireflyInputActionPriority : uint8
{
	Lowest = 0,
	Low = 1,
	Normal = 2,
	High = 3,
	Highest = 4
};


UCLASS(CollapseCategories, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class TIREFLYENHANCEDINPUT_API UTireflyInputActionConfig : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* InputAction = nullptr;;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETireflyInputActionPriority Priority = ETireflyInputActionPriority::Normal;
};


UCLASS()
class TIREFLYENHANCEDINPUT_API UTireflyGameplayInputConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;	
#endif

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 InputMappingContextPriority;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UTireflyInputActionConfig*> InputActionConfigs;
};
