// Copyright tzlFirefly. All Rights Reserved.


#include "Framework/Character/AsuraBaseCharacter.h"

// Sets default values
AAsuraBaseCharacter::AAsuraBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAsuraBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAsuraBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAsuraBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

