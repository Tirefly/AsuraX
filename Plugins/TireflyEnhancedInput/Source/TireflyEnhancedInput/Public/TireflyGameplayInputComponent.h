// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Components/ActorComponent.h"
#include "TireflyGameplayInputConfig.h"
#include "TireflyGameplayInputComponent.generated.h"


class UInputMappingContext;


USTRUCT(BlueprintType)
struct FTireflyInputBufferData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	UInputAction* InputAction = nullptr;

	UPROPERTY(BlueprintReadOnly)
	ETriggerEvent TriggerEvent = ETriggerEvent::None;

	UPROPERTY(BlueprintReadOnly)
	ETireflyInputActionPriority Priority = ETireflyInputActionPriority::Normal;
};


UCLASS(ClassGroup=(Input), meta=(BlueprintSpawnableComponent))
class TIREFLYENHANCEDINPUT_API UTireflyGameplayInputComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region EnhancedInputComponent

public:
	// Sets default values for this component's properties
	UTireflyGameplayInputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#pragma endregion


#pragma region GameplayInputBinding

protected:
	UFUNCTION()
	void HandlePlayerControllerPossessedPawn(APawn* OldPawn, APawn* NewPawn);
	
	UFUNCTION()
	bool AddInputMappingContext(const UInputMappingContext* Context, int32 InPriority = 0) const;

	UFUNCTION()
	void HandleGameplayInputBinding(const FInputActionInstance& Instance);

public:
	UFUNCTION(BlueprintCallable, Category = "Input")
	bool BindGameplayInputConfig(UTireflyGameplayInputConfig* InputConfig);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Input")
	TArray<UTireflyGameplayInputConfig*> GameplayInputConfigs;
	
	UPROPERTY(Transient)
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(Transient)
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent;

#pragma endregion


#pragma region InputBuffer

public:
	UFUNCTION(BlueprintCallable, Category = "Input")
	void StartCacheInputBuffer();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void StopCacheInputBuffer();

protected:
	UFUNCTION()
	void CacheInputBufferData(const FTireflyInputBufferData& NewInputBuffer);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Input")
	TArray<FTireflyInputBufferData> CachedInputBuffers;

	UPROPERTY(BlueprintReadOnly, Category = "Input")
	bool bInputBufferEnabled = false;

#pragma endregion
};
