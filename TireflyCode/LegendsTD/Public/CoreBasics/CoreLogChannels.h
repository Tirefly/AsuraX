// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"

LEGENDSTD_API DECLARE_LOG_CATEGORY_EXTERN(LogLegend, Log, All);
LEGENDSTD_API DECLARE_LOG_CATEGORY_EXTERN(LogLegendSynergy, Log, All);
LEGENDSTD_API DECLARE_LOG_CATEGORY_EXTERN(LogLegendRelationship, Log, All);
LEGENDSTD_API DECLARE_LOG_CATEGORY_EXTERN(LogLegendAbilitySystem, Log, All);

LEGENDSTD_API FString GetContextNetRoleString(UObject* ContextObject = nullptr);