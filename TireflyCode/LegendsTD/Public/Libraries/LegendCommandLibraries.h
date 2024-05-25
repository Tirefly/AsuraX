// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LegendCommandLibraries.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDSTD_API ULegendCommandLibraries : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//load file
	UFUNCTION(BlueprintCallable, category = "SHL FILE")
		static bool SHL_SaveIni(FString FileName, FString Type, FString Key, FString Value);

	UFUNCTION(BlueprintCallable, category = "SHL FILE")
		static FString SHL_LoadIni(FString FileName, FString Type, FString Key);
};
