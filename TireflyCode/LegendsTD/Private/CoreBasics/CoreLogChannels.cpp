// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreBasics/CoreLogChannels.h"

DEFINE_LOG_CATEGORY(LogLegend);
DEFINE_LOG_CATEGORY(LogLegendSynergy);
DEFINE_LOG_CATEGORY(LogLegendRelationship);
DEFINE_LOG_CATEGORY(LogLegendAbilitySystem);

FString GetContextNetRoleString(UObject* ContextObject)
{
	ENetRole Role = ROLE_None;

	if (AActor* Actor = Cast<AActor>(ContextObject))
	{
		Role = Actor->GetLocalRole();
	}
	else if (UActorComponent* Component = Cast<UActorComponent>(ContextObject))
	{
		Role = Component->GetOwnerRole();
	}

	if (Role != ROLE_None)
	{
		return (Role == ROLE_Authority) ? TEXT("Authority") :
			((Role == ROLE_AutonomousProxy) ? TEXT("Autonomous") : TEXT("Simulated"));
	}
	else
	{
#if WITH_EDITOR
		if (GIsEditor)
		{
			extern ENGINE_API FString GPlayInEditorContextString;
			return GPlayInEditorContextString;
		}
#endif
	}

	return TEXT("[]");
}
