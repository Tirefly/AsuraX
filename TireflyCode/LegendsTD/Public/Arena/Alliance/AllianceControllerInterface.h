// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AllianceControllerInterface.generated.h"


class UAllianceManagerComponent;


// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UAllianceControllerInterface : public UInterface
{
	GENERATED_BODY()
};


// 羁绊管理器组件所挂载的Controller需要继承的接口
class LEGENDSTD_API IAllianceControllerInterface
{
	GENERATED_BODY()
		
public:
	/** 获取Controller的羁绊管理器组件 */
	UFUNCTION(BlueprintCallable, Category = "Controller|Alliance")
	virtual UAllianceManagerComponent* GetAllianceManager() const { return nullptr; }
};
