// Copyright Tirefly. All Rights Reserved.

#include "TireflyObjectPoolWorldSubsystem.h"

#include "Engine/World.h"
#include "TimerManager.h"


TMap<TSubclassOf<AActor>, UTireflyObjectPoolWorldSubsystem::TActorPoolList> UTireflyObjectPoolWorldSubsystem::ActorPoolOfClass;
TMap<FName, UTireflyObjectPoolWorldSubsystem::TActorPoolList> UTireflyObjectPoolWorldSubsystem::ActorPoolOfID;


void UTireflyObjectPoolWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UTireflyObjectPoolWorldSubsystem::Deinitialize()
{
	ActorPool_ClearAll();

	Super::Deinitialize();
}

void UTireflyObjectPoolWorldSubsystem::ActorPool_ClearAll()
{
	for (auto Pool : ActorPoolOfClass)
	{
		for (auto Actor : Pool.Value)
		{
			if (IsValid(Actor))
			{
				Actor->Destroy(true);
			}
		}
	}

	for (auto Pool : ActorPoolOfID)
	{
		for (auto Actor : Pool.Value)
		{
			if (IsValid(Actor))
			{
				Actor->Destroy(true);
			}
		}
	}

	ActorPoolOfClass.Empty();
	ActorPoolOfID.Empty();
}

void UTireflyObjectPoolWorldSubsystem::ActorPool_ClearByClass(TSubclassOf<AActor> ActorClass)
{
	if (TActorPoolList* Pool = ActorPoolOfClass.Find(ActorClass))
	{
		for (auto Actor : *Pool)
		{
			if (IsValid(Actor))
			{
				Actor->Destroy(true);
			}
		}
		Pool->Empty();
		ActorPoolOfClass.Remove(ActorClass);
	}
}

void UTireflyObjectPoolWorldSubsystem::ActorPool_ClearByID(FName ActorID)
{
	if (TActorPoolList* Pool = ActorPoolOfID.Find(ActorID))
	{
		for (auto Actor : *Pool)
		{
			if (IsValid(Actor))
			{
				Actor->Destroy(true);
			}
		}
		Pool->Empty();
		ActorPoolOfID.Remove(ActorID);
	}
}

AActor* UTireflyObjectPoolWorldSubsystem::K2_ActorPool_FetchActor(TSubclassOf<AActor> ActorClass, FName ActorID)
{
	return ActorPool_FetchActor<AActor>(ActorClass, ActorID);
}

TArray<AActor*> UTireflyObjectPoolWorldSubsystem::K2_ActorPool_FetchActors(TSubclassOf<AActor> ActorClass, FName ActorID,
                                                                           int32 Count)
{
	return ActorPool_FetchActors<AActor>(ActorClass, ActorID, Count);
}

AActor* UTireflyObjectPoolWorldSubsystem::SpawnActor_Internal(TSubclassOf<AActor> ActorClass, FName ActorID,
	const FTransform& Transform, float Lifetime, AActor* Owner, APawn* Instigator,
	const ESpawnActorCollisionHandlingMethod CollisionHandling)
{
	UWorld* World = GetWorld();
	if (!IsValid(World) || (!IsValid(ActorClass) && ActorID == NAME_None))
	{
		return nullptr;
	}

	AActor* Actor = ActorPool_FetchActor<AActor>(ActorClass, ActorID);
	if (Actor)
	{
		Actor->SetActorTransform(Transform, true, nullptr, ETeleportType::ResetPhysics);
		Actor->SetOwner(Owner);

		if (Actor->template Implements<UTireflyPoolingActorInterface>())
		{
			if (ITireflyPoolingActorInterface::Execute_PoolingGetActorID(Actor) != ActorID && ActorID != NAME_None)
			{
				ITireflyPoolingActorInterface::Execute_PoolingSetActorID(Actor, ActorID);
			}
			ITireflyPoolingActorInterface::Execute_PoolingBeginPlay(Actor);
		}
	}
	else
	{
		if (!IsValid(ActorClass))
		{
			return nullptr;
		}

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = Owner;
		SpawnParameters.Instigator = Instigator;
		SpawnParameters.SpawnCollisionHandlingOverride = CollisionHandling;

		Actor = World->SpawnActor<AActor>(ActorClass, Transform, SpawnParameters);
		if (Actor->template Implements<UTireflyPoolingActorInterface>())
		{
			if (ActorID != NAME_None)
			{
				ITireflyPoolingActorInterface::Execute_PoolingSetActorID(Actor, ActorID);
			}
			ITireflyPoolingActorInterface::Execute_PoolingBeginPlay(Actor);
		}
	}

	if (IsValid(Actor) && Lifetime > 0.f)
	{
		FTimerHandle TimerHandle;
		auto TimerLambda = [Actor]() { ActorPool_ReleaseActor(Actor); };
		World->GetTimerManager().SetTimer(TimerHandle, TimerLambda, Lifetime, false);
	}

	return Actor;
}

AActor* UTireflyObjectPoolWorldSubsystem::ActorPool_BeginDeferredActorSpawn(const UObject* WorldContext, TSubclassOf<AActor> ActorClass
	, FName ActorID, const FTransform& SpawnTransform, AActor* Owner, ESpawnActorCollisionHandlingMethod CollisionHandling)
{
	UWorld* World = WorldContext->GetWorld();
	if (!IsValid(World) || (!IsValid(ActorClass) && ActorID == NAME_None))
	{
		return nullptr;
	}

	auto SetActorID = [ActorID](AActor* InActor)
	{
		if (InActor->Implements<UTireflyPoolingActorInterface>())
		{
			if (ITireflyPoolingActorInterface::Execute_PoolingGetActorID(InActor) != ActorID && ActorID != NAME_None)
			{
				ITireflyPoolingActorInterface::Execute_PoolingSetActorID(InActor, ActorID);
			}
		}
	};

	AActor* Actor = ActorPool_FetchActor<AActor>(ActorClass, ActorID);
	if (Actor)
	{
		SetActorID(Actor);
		Actor->SetActorTransform(SpawnTransform, true, nullptr, ETeleportType::ResetPhysics);
		Actor->SetOwner(Owner);

		return Actor;
	}

	if (!IsValid(ActorClass))
	{
		return nullptr;
	}

	UObject* MutableWorldContext = const_cast<UObject*>(WorldContext);
	APawn* AutoInstigator = Cast<APawn>(MutableWorldContext);
	Actor = World->SpawnActorDeferred<AActor>(ActorClass, SpawnTransform, Owner, AutoInstigator, CollisionHandling);
	SetActorID(Actor);

	return Actor;
}

AActor* UTireflyObjectPoolWorldSubsystem::ActorPool_FinishSpawningActor(const UObject* WorldContext, AActor* Actor
	, const FTransform& SpawnTransform, float Lifetime)
{
	UWorld* World = WorldContext->GetWorld();
	if (!IsValid(World) || !IsValid(Actor))
	{
		return nullptr;
	}

	if ((!Actor->IsActorInitialized()))
	{
		Actor->FinishSpawning(SpawnTransform);
	}

	if (Actor->Implements<UTireflyPoolingActorInterface>())
	{
		ITireflyPoolingActorInterface::Execute_PoolingBeginPlay(Actor);
	}

	if (Lifetime > 0.f)
	{
		FTimerHandle TimerHandle;
		auto TimerLambda = [Actor]() { ActorPool_ReleaseActor(Actor); };
		World->GetTimerManager().SetTimer(TimerHandle, TimerLambda, Lifetime, false);
	}

	return Actor;
}

void UTireflyObjectPoolWorldSubsystem::ActorPool_ReleaseActor(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return;
	}

	FName ActorID = NAME_None;
	if (Actor->Implements<UTireflyPoolingActorInterface>())
	{
		ActorID = ITireflyPoolingActorInterface::Execute_PoolingGetActorID(Actor);
		ITireflyPoolingActorInterface::Execute_PoolingEndPlay(Actor);
	}

	if (ActorID != NAME_None)
	{
		TActorPoolList& Pool = ActorPoolOfID.FindOrAdd(ActorID);
		Pool.Push(Actor);

		return;
	}

	TActorPoolList& Pool = ActorPoolOfClass.FindOrAdd(Actor->GetClass());	
	Pool.Push(Actor);	
}

void UTireflyObjectPoolWorldSubsystem::ActorPool_WarmUp(const UObject* WorldContextObject,
	TSubclassOf<AActor> ActorClass, FName ActorID, const FTransform& Transform, AActor* Owner, APawn* Instigator,
	int32 Count)
{
	UWorld* World = WorldContextObject->GetWorld();

	if (!IsValid(World) || !IsValid(ActorClass) || ActorID == NAME_None || Count <= 0)
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Owner;
	SpawnParameters.Instigator = Instigator;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TActorPoolList& Pool = ActorID != NAME_None ? ActorPoolOfID.FindOrAdd(ActorID) : ActorPoolOfClass.FindOrAdd(ActorClass);
	Pool.Reserve(Count);
	for (int32 i = 0; i < Count; i++)
	{
		AActor* Actor = World->SpawnActor<AActor>(ActorClass, Transform, SpawnParameters);
		if (Actor->Implements<UTireflyPoolingActorInterface>())
		{
			if (ActorID != NAME_None)
			{
				ITireflyPoolingActorInterface::Execute_PoolingSetActorID(Actor, ActorID);
			}
			ITireflyPoolingActorInterface::Execute_PoolingWarmUp(Actor);
		}

		Pool.Push(Actor);
	}
}

TArray<TSubclassOf<AActor>> UTireflyObjectPoolWorldSubsystem::ActorPool_DebugActorClasses()
{
	TArray<TSubclassOf<AActor>> ActorClasses;
	ActorPoolOfClass.GetKeys(ActorClasses);

	return ActorClasses;
}

TArray<FName> UTireflyObjectPoolWorldSubsystem::ActorPool_DebugActorIDs()
{
	TArray<FName> ActorIDs;
	ActorPoolOfID.GetKeys(ActorIDs);

	return ActorIDs;
}

int32 UTireflyObjectPoolWorldSubsystem::ActorPool_DebugActorNumberOfClass(TSubclassOf<AActor> ActorClass)
{
	if (!ActorPoolOfClass.Contains(ActorClass))
	{
		return -1;
	}

	return ActorPoolOfClass[ActorClass].Num();
}

int32 UTireflyObjectPoolWorldSubsystem::ActorPool_DebugActorNumberOfID(FName ActorID)
{
	if (!ActorPoolOfID.Contains(ActorID))
	{
		return -1;
	}

	return ActorPoolOfID[ActorID].Num();
}
