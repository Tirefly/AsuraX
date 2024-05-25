// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "Arena/CombatExtra/CombatDomain.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "FireflyObjectPoolLibrary.h"
#include "FireflyObjectPoolWorldSubsystem.h"
#include "NiagaraComponent.h"
#include "Arena/CombatUnits/CombatUnitBase.h"
#include "Libraries/LegendTraceLibrary.h"


ACombatDomain::ACombatDomain()
{
 	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DomainRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DomainRoot"));
	if (IsValid(DomainRoot))
	{
		SetRootComponent(DomainRoot);
	}

	DomainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DomainMesh"));
	if (IsValid(DomainMesh))
	{
		DomainMesh->SetupAttachment(DomainRoot);
		DomainMesh->SetGenerateOverlapEvents(false);
		DomainMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DomainMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	DomainNS = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DomainNS"));
	if (IsValid(DomainNS))
	{
		DomainNS->SetupAttachment(DomainRoot);
	}
}

// Called when the game starts or when spawned
void ACombatDomain::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACombatDomain::PoolingBeginPlay_Implementation()
{
	UFireflyObjectPoolLibrary::UniversalBeginPlay_Actor(this, this);
}

void ACombatDomain::PoolingEndPlay_Implementation()
{
	if (bFollowInstigator)
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	HandleDomainPendingDestroy();
	GetWorldTimerManager().ClearTimer(DomainExecutionTimer);

	UFireflyObjectPoolLibrary::UniversalEndPlay_Actor(this, this);
}

void ACombatDomain::InitDomainOnRespawn(AActor* InInstigator, const FVector& Range)
{
	if (!IsValid(InInstigator) || Range == FVector::ZeroVector)
	{
		GetWorld()->GetSubsystem<UFireflyObjectPoolWorldSubsystem>()->ActorPool_ReleaseActor(this);
		return;
	}

	DomainInstigator = InInstigator;
	if (const ACombatUnitBase* CombatUnit = Cast<ACombatUnitBase>(DomainInstigator))
	{
		DomainInstigatorAlliance = CombatUnit->GetCurrentAlliance();
	}

	DomainRange = Range;

	ExecuteDomainCheck();
	GetWorldTimerManager().SetTimer(DomainExecutionTimer, this
		, &ACombatDomain::ExecuteDomainCheck, 1.0f, true);
}

void ACombatDomain::InitDomainSkin() const
{
	DomainMesh->SetStaticMesh(DomainSkin.ExtraMesh);
	DomainMesh->SetMaterial(0, DomainSkin.ExtraMeshSkin);
	DomainNS->SetAsset(DomainSkin.ExtraNS);
}

void ACombatDomain::ExecuteDomainCheck_Implementation()
{
	TArray<AActor*> ActorsChecked;
	TraceCheckDomainTargets(ActorsChecked);

	TArray<AActor*> OverlappingActors;
	for (auto Actor : ActorsChecked)
	{
		// 获取碰撞的物体
		OverlappingActors.AddUnique(Actor);
		// 检查碰撞的物体是否是Pawn或者物体是否在域内
		if (ActorsInDomain.Contains(Actor))
		{
			continue;
		}

		if (ACombatUnitBase* CombatUnit = Cast<ACombatUnitBase>(Actor))
		{
			// 检查碰撞的物体是否是当前玩家的朋友
			if (DomainInstigatorAlliance == CombatUnit->GetCurrentAlliance())
			{
				OnAllyEnterDomain(CombatUnit);
			}
			else
			{
				OnEnemyEnterDomain(CombatUnit);
			}
		}

		ActorsInDomain.Add(Actor);
	}

	// 检查域内的物体是否被移除
	TArray<AActor*> ActorExited;
	for (auto Actor : ActorsInDomain)
	{
		// 检查当前物体是否在碰撞列表中
		if (OverlappingActors.Contains(Actor))
		{
			continue;
		}

		if (ACombatUnitBase* CombatUnit = Cast<ACombatUnitBase>(Actor))
		{
			// 检查当前物体是否是当前玩家的朋友
			if (DomainInstigatorAlliance == CombatUnit->GetCurrentAlliance())
			{
				OnAllyExitedDomain(CombatUnit);
			}
			else
			{
				OnEnemyExitedDomain(CombatUnit);
			}
		}

		ActorExited.Emplace(Actor);
	}

	for (AActor* Actor : ActorExited)
	{
		ActorsInDomain.Remove(Actor);
	}
}

bool ACombatDomain::TraceCheckDomainTargets_Implementation(TArray<AActor*>& OutActors)
{
	const ACombatUnitBase* CombatUnit = Cast<ACombatUnitBase>(DomainInstigator);
	if (!IsValid(CombatUnit))
	{
		return false;
	}

	AController* Alliance = CombatUnit->GetCurrentAlliance();
	if (!IsValid(Alliance))
	{
		return false;
	}

	TArray<AActor*> Enemies;
	TArray<AActor*> Allies;
	if (FMath::IsNearlyZero(DomainRange.Y) && FMath::IsNearlyZero(DomainRange.Z))
	{
		ULegendTraceLibrary::SphereTraceTargetInGrid(this, this, Alliance, DomainRange.X, Enemies, false);
		ULegendTraceLibrary::SphereTraceTargetInGrid(this, this, Alliance, DomainRange.X, Allies, true);
		OutActors.Append(Enemies);
		OutActors.Append(Allies);

		return true;
	}

	if (FMath::IsNearlyZero(DomainRange.Z))
	{
		ULegendTraceLibrary::SectorTraceTargetInGrid(this, this, Alliance, DomainRange.X, DomainRange.Y, Enemies, false);
		ULegendTraceLibrary::SectorTraceTargetInGrid(this, this, Alliance, DomainRange.X, DomainRange.Y, Allies, true);
		OutActors.Append(Enemies);
		OutActors.Append(Allies);

		return true;
	}

	ULegendTraceLibrary::BoxTraceTargetInGrid(this, this, Alliance, DomainRange, Enemies, false);
	ULegendTraceLibrary::BoxTraceTargetInGrid(this, this, Alliance, DomainRange, Allies, true);
	OutActors.Append(Enemies);
	OutActors.Append(Allies);

	return true;
}

void ACombatDomain::HandleDomainPendingDestroy_Implementation()
{
	for (AActor* ActorInRange : ActorsInDomain)
	{
		if (DomainInstigatorAlliance == Cast<ACombatUnitBase>(ActorInRange)->GetCurrentAlliance())
		{
			HandleAllyInRangeOnDomainPendingDestroy(ActorInRange);
			continue;
		}

		HandleEnemyInRangeOnDomainPendingDestroy(ActorInRange);
	}

	ActorsInDomain.Empty();
}

void ACombatDomain::OnEnemyEnterDomain_Implementation(AActor* NewEnemy)
{
	if (!IsValid(NewEnemy))
	{
		return;
	}

	UAbilitySystemComponent* InstigatorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(DomainInstigator);
	if (!IsValid(InstigatorASC))
	{
		return;
	}

	for (FGameplayEffectSpecHandle Effect : DomainEffect.EffectsToEnemiesOnEnter)
	{
		if (UAbilitySystemComponent * TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(NewEnemy))
		{
			InstigatorASC->BP_ApplyGameplayEffectSpecToTarget(Effect, TargetASC);
		}
	}
}

void ACombatDomain::OnEnemyExitedDomain_Implementation(AActor* EnemyExited)
{
	if (!IsValid(EnemyExited))
	{
		return;
	}

	UAbilitySystemComponent* InstigatorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(DomainInstigator);
	if (!IsValid(InstigatorASC))
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(EnemyExited);
	if (!IsValid(TargetASC))
	{
		return;
	}

	for (FGameplayEffectSpecHandle Effect : DomainEffect.EffectsToEnemiesOnEnter)
	{
		if (const FGameplayEffectSpec* EffectDef = Effect.Data.Get())
		{
			if (const TSubclassOf<UGameplayEffect> EffectClass = EffectDef->Def.Get()->GetClass())
			{
				TargetASC->RemoveActiveGameplayEffectBySourceEffect(EffectClass, InstigatorASC);
			}
		}
	}

	for (FGameplayEffectSpecHandle Effect : DomainEffect.EffectsToEnemiesOnExit)
	{
		InstigatorASC->BP_ApplyGameplayEffectSpecToTarget(Effect, TargetASC);
	}
}

void ACombatDomain::HandleEnemyInRangeOnDomainPendingDestroy_Implementation(AActor* Enemy)
{
	if (!IsValid(Enemy))
	{
		return;
	}

	UAbilitySystemComponent* InstigatorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(DomainInstigator);
	if (!IsValid(InstigatorASC))
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Enemy);
	if (!IsValid(TargetASC))
	{
		return;
	}

	for (FGameplayEffectSpecHandle Effect : DomainEffect.EffectsToEnemiesOnEnter)
	{
		if (const FGameplayEffectSpec* EffectDef = Effect.Data.Get())
		{
			if (const TSubclassOf<UGameplayEffect> EffectClass = EffectDef->Def.Get()->GetClass())
			{
				TargetASC->RemoveActiveGameplayEffectBySourceEffect(EffectClass, InstigatorASC);
			}
		}
	}

	for (FGameplayEffectSpecHandle Effect : DomainEffect.EffectsToEnemiesOnDestruction)
	{
		InstigatorASC->BP_ApplyGameplayEffectSpecToTarget(Effect, TargetASC);
	}
}

void ACombatDomain::OnAllyEnterDomain_Implementation(AActor* NewAlly)
{
	if (!IsValid(NewAlly))
	{
		return;
	}

	UAbilitySystemComponent* InstigatorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(DomainInstigator);
	if (!IsValid(InstigatorASC))
	{
		return;
	}

	for (FGameplayEffectSpecHandle Effect : DomainEffect.EffectsToAlliesOnEnter)
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(NewAlly))
		{
			InstigatorASC->BP_ApplyGameplayEffectSpecToTarget(Effect, TargetASC);
		}
	}
}

void ACombatDomain::OnAllyExitedDomain_Implementation(AActor* AllyExited)
{
	if (!IsValid(AllyExited))
	{
		return;
	}

	UAbilitySystemComponent* InstigatorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(DomainInstigator);
	if (!IsValid(InstigatorASC))
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AllyExited);
	if (!IsValid(TargetASC))
	{
		return;
	}

	for (FGameplayEffectSpecHandle Effect : DomainEffect.EffectsToAlliesOnEnter)
	{
		if (const FGameplayEffectSpec* EffectDef = Effect.Data.Get())
		{
			if (const TSubclassOf<UGameplayEffect> EffectClass = EffectDef->Def.Get()->GetClass())
			{
				TargetASC->RemoveActiveGameplayEffectBySourceEffect(EffectClass, InstigatorASC);
			}
		}
	}

	for (FGameplayEffectSpecHandle Effect : DomainEffect.EffectsToAlliesOnExit)
	{
		InstigatorASC->BP_ApplyGameplayEffectSpecToTarget(Effect, TargetASC);
	}
}

void ACombatDomain::HandleAllyInRangeOnDomainPendingDestroy_Implementation(AActor* Ally)
{
	if (!IsValid(Ally))
	{
		return;
	}

	UAbilitySystemComponent* InstigatorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(DomainInstigator);
	if (!IsValid(InstigatorASC))
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Ally);
	if (!IsValid(TargetASC))
	{
		return;
	}

	for (FGameplayEffectSpecHandle Effect : DomainEffect.EffectsToAlliesOnEnter)
	{
		if (const FGameplayEffectSpec* EffectDef = Effect.Data.Get())
		{
			if (const TSubclassOf<UGameplayEffect> EffectClass = EffectDef->Def.Get()->GetClass())
			{
				TargetASC->RemoveActiveGameplayEffectBySourceEffect(EffectClass, InstigatorASC);
			}
		}
	}

	for (FGameplayEffectSpecHandle Effect : DomainEffect.EffectsToAlliesOnDestruction)
	{
		InstigatorASC->BP_ApplyGameplayEffectSpecToTarget(Effect, TargetASC);
	}
}
