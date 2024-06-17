// Copyright Tirefly. All Rights Reserved.


#include "TireflyGameplayInputComponent.h"

#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
UTireflyGameplayInputComponent::UTireflyGameplayInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

// Called when the game starts
void UTireflyGameplayInputComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = GetOwner()->IsA(APlayerController::StaticClass()) ? Cast<APlayerController>(GetOwner())
		: (GetOwner()->IsA(APawn::StaticClass()) ? Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController()) : nullptr);
	if (PlayerController)
	{
		PlayerController->OnPossessedPawnChanged.AddDynamic(this, &UTireflyGameplayInputComponent::HandlePlayerControllerPossessedPawn);
		HandlePlayerControllerPossessedPawn(nullptr, PlayerController->GetPawn());
	}
}

void UTireflyGameplayInputComponent::HandlePlayerControllerPossessedPawn(APawn* OldPawn, APawn* NewPawn)
{
	if (!NewPawn)
	{
		return;
	}

	EnhancedInputComponent = Cast<UEnhancedInputComponent>(NewPawn->InputComponent);
}

bool UTireflyGameplayInputComponent::AddInputMappingContext(const UInputMappingContext* Context, int32 InPriority) const
{
	if (!PlayerController)
	{
		return false;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer)
	{
		return false;
	}

	UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!LocalPlayerSubsystem)
	{
		return false;
	}

	LocalPlayerSubsystem->AddMappingContext(Context, InPriority);

	return true;
}

void UTireflyGameplayInputComponent::HandleGameplayInputBinding(const FInputActionInstance& Instance)
{
	if (!Instance.GetSourceAction())
	{
		return;
	}

	FTireflyInputBufferData NewInputBuffer = FTireflyInputBufferData();
	NewInputBuffer.InputAction = const_cast<UInputAction*>(Instance.GetSourceAction());
	NewInputBuffer.TriggerEvent = Instance.GetTriggerEvent();
	for (const UTireflyGameplayInputConfig* InputConfig : GameplayInputConfigs)
	{
		for (const UTireflyInputActionConfig* InputActionConfig : InputConfig->InputActionConfigs)
		{
			if (InputActionConfig->InputAction == NewInputBuffer.InputAction)
			{
				NewInputBuffer.Priority = InputActionConfig->Priority;
				break;
			}
		}
	}
	CacheInputBufferData(NewInputBuffer);
}

bool UTireflyGameplayInputComponent::BindGameplayInputConfig(UTireflyGameplayInputConfig* InputConfig)
{
	if (!InputConfig || !GetOwner() || !EnhancedInputComponent)
	{
		return false;
	}

	if (!AddInputMappingContext(InputConfig->InputMappingContext, InputConfig->InputMappingContextPriority))
	{
		return false;
	}

	for (const UTireflyInputActionConfig* InputActionConfig : InputConfig->InputActionConfigs)
	{
		if (!InputActionConfig || !InputActionConfig->InputAction)
		{
			continue;
		}

		EnhancedInputComponent->BindAction(InputActionConfig->InputAction, ETriggerEvent::Started, this,
			&UTireflyGameplayInputComponent::HandleGameplayInputBinding);
		EnhancedInputComponent->BindAction(InputActionConfig->InputAction, ETriggerEvent::Ongoing, this,
			&UTireflyGameplayInputComponent::HandleGameplayInputBinding);
		EnhancedInputComponent->BindAction(InputActionConfig->InputAction, ETriggerEvent::Canceled, this,
			&UTireflyGameplayInputComponent::HandleGameplayInputBinding);
		EnhancedInputComponent->BindAction(InputActionConfig->InputAction, ETriggerEvent::Triggered, this,
			&UTireflyGameplayInputComponent::HandleGameplayInputBinding);
		EnhancedInputComponent->BindAction(InputActionConfig->InputAction, ETriggerEvent::Completed, this,
			&UTireflyGameplayInputComponent::HandleGameplayInputBinding);
	}

	return true;
}

void UTireflyGameplayInputComponent::StartCacheInputBuffer()
{
	bInputBufferEnabled = true;
	CachedInputBuffers.Empty();
}

void UTireflyGameplayInputComponent::StopCacheInputBuffer()
{
	bInputBufferEnabled = false;
	CachedInputBuffers.Empty();
}

void UTireflyGameplayInputComponent::CacheInputBufferData(const FTireflyInputBufferData& NewInputBuffer)
{
	if (!bInputBufferEnabled)
	{
		return;
	}

	CachedInputBuffers.Add(NewInputBuffer);
	Algo::Sort(CachedInputBuffers, [](const FTireflyInputBufferData& A, const FTireflyInputBufferData& B)
	{
		return A.Priority > B.Priority;
	});
}
