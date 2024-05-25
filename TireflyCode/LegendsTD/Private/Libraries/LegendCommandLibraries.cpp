// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "Libraries/LegendCommandLibraries.h"

bool ULegendCommandLibraries::SHL_SaveIni(FString FileName,FString Type, FString Key, FString Value)
{
	if (GConfig)
	{
		GConfig->SetString(*Type,*Key,*Value,*FileName);

		GConfig->Flush(1, FileName);
		return true;
	}
	return false;
	
}

FString ULegendCommandLibraries::SHL_LoadIni(FString FileName, FString Type, FString Key)
{
    //FConfigCacheIni* mconfig=NewObject<FConfigCacheIni>;
    GConfig->LoadFile(FileName);
    FString Value;
    GConfig->GetString(
           *Type,
           *Key,
           Value,
           FileName
    );
    return Value;
}