// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "Arena/CombatExtra/CombatProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "FireflyObjectPoolLibrary.h"
#include "FireflyObjectPoolWorldSubsystem.h"
#include "NiagaraComponent.h"
#include "Arena/CombatUnits/CombatUnitBase.h"
#include "Libraries/LegendTraceLibrary.h"


ACombatProjectile::ACombatProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	ProjectileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileRoot"));
	if (IsValid(ProjectileRoot))
	{
		SetRootComponent(ProjectileRoot);
	}

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	if (IsValid(ProjectileMesh))
	{
		ProjectileMesh->SetupAttachment(ProjectileRoot);
		ProjectileMesh->SetGenerateOverlapEvents(false);
		ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	ProjectileNS = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNS"));
	if (IsValid(ProjectileNS))
	{
		ProjectileNS->SetupAttachment(ProjectileRoot);
	}
}

void ACombatProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ACombatProjectile::PoolingBeginPlay_Implementation()
{
	UFireflyObjectPoolLibrary::UniversalBeginPlay_Actor(this, this);
}

void ACombatProjectile::PoolingEndPlay_Implementation()
{
	UFireflyObjectPoolLibrary::UniversalEndPlay_Actor(this, this);
}

void ACombatProjectile::InitProjectileSkin() const
{
	ProjectileMesh->SetStaticMesh(ProjectileSkin.ExtraMesh);
	ProjectileMesh->SetMaterial(0, ProjectileSkin.ExtraMeshSkin);
	ProjectileNS->SetAsset(ProjectileSkin.ExtraNS);
}

void ACombatProjectile::InitProjectileOnRespawn(UAbilitySystemComponent* OwnerASC, AActor* InTargetActor,
	FVector InTargetLocation, float InExplodingRange)
{
	if (!IsValid(OwnerASC) || (!IsValid(InTargetActor) && InTargetLocation == FVector::ZeroVector))
	{
		GetWorld()->GetSubsystem<UFireflyObjectPoolWorldSubsystem>()->ActorPool_ReleaseActor(this);
		return;
	}

	InstigatorASC = OwnerASC;
	TargetActor = InTargetActor;
	TargetLocation = InTargetLocation;
	ExplodingRange = InExplodingRange;

	OriginLocation = GetActorLocation();
	StartShoot();
}

void ACombatProjectile::FinishShoot_Implementation()
{
	if (ExplodingRange > 0.f)
	{
		const ACombatUnitBase* AvatarOwner = Cast<ACombatUnitBase>(InstigatorASC->GetAvatarActor());
		if (IsValid(AvatarOwner))
		{
			TArray<AActor*> HitTargets;
			if (ULegendTraceLibrary::SphereTraceTargetInGrid(this, this
				, AvatarOwner->GetCurrentAlliance(), ExplodingRange, HitTargets))
			{
				for (AActor* Target : HitTargets)
				{
					ApplyEffectToTarget(Target);
				}
			}
		}
	}
	else if (IsValid(TargetActor))
	{
		ApplyEffectToTarget(TargetActor);
	}

	GetWorld()->GetSubsystem<UFireflyObjectPoolWorldSubsystem>()->ActorPool_ReleaseActor(this);
}

void ACombatProjectile::ApplyEffectToTarget_Implementation(AActor* InTarget)
{
	if (!IsValid(InstigatorASC) || !IsValid(InTarget))
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InTarget);
	if (!IsValid(TargetASC))
	{
		return;
	}

	for (FGameplayEffectSpecHandle Handle : ProjectileEffect.EffectsToTarget)
	{
		InstigatorASC->BP_ApplyGameplayEffectSpecToTarget(Handle, TargetASC);
	}
}
