// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbilities/AbilityParameter/TireflyAbilityParam_ActorSpawn.h"

#include "FireflyAbilitySystemComponent.h"
#include "FireflyObjectPoolWorldSubsystem.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Numeric.h"
#include "Kismet/GameplayStatics.h"


AActor* UTireflyAbilityParam_ActorSpawn::ExecuteActorSpawn_Implementation(const UObject* WorldContext,
	TSubclassOf<AActor> InActorClass, FTransform InSpawnTransform, FTireflyAbilityParamInfo ParamInfo,
	AActor* Owner, APawn* Instigator)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return nullptr;
	}

	AActor* OutActor;
	FTransform ActorSpawnTransform = SpawnTransform ? SpawnTransform->GetActorSpawnTransform(ParamInfo) : InSpawnTransform;
	if (bUseObjectPool)
	{
		UFireflyObjectPoolWorldSubsystem* SubsystemOP = World->GetSubsystem<UFireflyObjectPoolWorldSubsystem>();
		if (!SubsystemOP)
		{
			return nullptr;
		}
		OutActor = SubsystemOP->ActorPool_SpawnActor(InActorClass, ActorId,	ActorSpawnTransform,
			LifetimeForObjectPool ? LifetimeForObjectPool->GetParamValue(ParamInfo) : -1.f,
			Owner, Instigator, SpawnCollisionHandling);
	}
	else
	{
		OutActor = World->SpawnActorDeferred<AActor>(InActorClass, ActorSpawnTransform,
			Owner, Instigator, SpawnCollisionHandling);
		if (OutActor->Implements<UFireflyPoolingActorInterface>())
		{
			if (IFireflyPoolingActorInterface::Execute_PoolingGetActorID(OutActor) != ActorId && ActorId != NAME_None)
			{
				IFireflyPoolingActorInterface::Execute_PoolingSetActorID(OutActor, ActorId);
			}
			IFireflyPoolingActorInterface::Execute_PoolingBeginPlay(OutActor);
		}
		UGameplayStatics::FinishSpawningActor(OutActor, ActorSpawnTransform);
	}

	if (AttachmentRule)
	{
		AttachmentRule->HandleActorAttached(OutActor, ParamInfo);
	}

	for (auto& Detail : SpawnDetails)
	{
		Detail->HandleActorSpawned(OutActor, ParamInfo);
	}

	return OutActor;
}

FTransform UActorSpawnTransformDetail_ActorBasedTransform::GetActorSpawnTransform_Implementation(
	FTireflyAbilityParamInfo ParamInfo)
{
	AActor* SpawnBaseActor = nullptr;
	if (SpawnSource == ETireflyAbilityParamSourceType::Caster)
	{
		if (ParamInfo.CasterASC)
		{
			SpawnBaseActor = ParamInfo.CasterASC->GetOwner();
		}
	}
	else
	{
		if (ParamInfo.TargetASC)
		{
			SpawnBaseActor = ParamInfo.TargetASC->GetOwner();
		}
	}
	if (!SpawnBaseActor)
	{
		return FTransform();
	}

	FTransform OutSpawnTransform = Super::GetActorSpawnTransform_Implementation(ParamInfo);
	if (SpawnBaseBone != NAME_None)
	{
		TArray<UPrimitiveComponent*> Components;
		SpawnBaseActor->GetComponents<UPrimitiveComponent>(Components);
		for (auto& Component : Components)
		{
			if (Component->DoesSocketExist(SpawnBaseBone))
			{
				OutSpawnTransform = Component->GetSocketTransform(SpawnBaseBone);
				break;
			}
		}
	}

	OutSpawnTransform += OffsetFromSource;
	return OutSpawnTransform;
}

FAttachmentTransformRules UTireflyAbilityParamDetail_ActorAttachment::GetAttachmentTransformRules() const
{
	if (AttachmentRule == "Keep Relative Transform")
	{
		return FAttachmentTransformRules::KeepRelativeTransform;
	}
	if (AttachmentRule == "Keep World Transform")
	{
		return FAttachmentTransformRules::KeepWorldTransform;
	}
	if (AttachmentRule == "Snap To Target Not Including Scale")
	{
		return FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	}
	if (AttachmentRule == "Snap To Target Including Scale")
	{
		return FAttachmentTransformRules::SnapToTargetIncludingScale;
	}

	return FAttachmentTransformRules::KeepRelativeTransform;
}

void UTireflyAbilityParamDetail_ActorAttachment::HandleActorAttached_Implementation(AActor* SpawnedActor,
                                                                                    FTireflyAbilityParamInfo ParamInfo)
{
	AActor* AttachBaseActor = nullptr;
	if (AttachSource == ETireflyAbilityParamSourceType::Caster)
	{
		if (ParamInfo.CasterASC)
		{
			AttachBaseActor = ParamInfo.CasterASC->GetOwner();
		}
	}
	else
	{
		if (ParamInfo.TargetASC)
		{
			AttachBaseActor = ParamInfo.TargetASC->GetOwner();
		}
	}
	if (AttachBaseActor)
	{
		if (AttachBone != NAME_None)
		{
			TArray<UPrimitiveComponent*> Components;
			AttachBaseActor->GetComponents<UPrimitiveComponent>(Components);
			for (auto& Component : Components)
			{
				if (Component->DoesSocketExist(AttachBone))
				{
					SpawnedActor->AttachToComponent(Component, GetAttachmentTransformRules(), AttachBone);
					SpawnedActor->SetActorRelativeTransform(AttachOffset);
					break;
				}
			}
		}
		else
		{
			SpawnedActor->AttachToComponent(AttachBaseActor->GetRootComponent(), GetAttachmentTransformRules());
			SpawnedActor->SetActorRelativeTransform(AttachOffset);
		}
	}
}
