// Copyright Tirefly. All Rights Reserved.


#include "TireflyNameOptionPinFactory.h"

#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "SGraphPinNameList.h"


TSharedPtr<SGraphPin> FTireflyNameOptionPinFactory::CreatePin(UEdGraphPin* InPin) const
{
    // 检查节点引脚类型是否为名称类型
	if (InPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Name)
	{
		return nullptr;
	}

    // 尝试将节点引脚的外部对象转换为函数调用节点
	UK2Node_CallFunction* CallFunctionNode = Cast<UK2Node_CallFunction>(InPin->GetOuter());
	if (!CallFunctionNode)
	{
		return nullptr;
	}

    // 从函数调用节点的元数据中获取参数选项的函数名
	FString PinOptionFunctionName = CallFunctionNode->GetPinMetaData(InPin->PinName, FName("GetParamOptions"));
	if (PinOptionFunctionName.IsEmpty())
	{
		return nullptr;
	}

	UObject* OwningObject = nullptr;
    // 处理具有命名空间的函数名
	if (PinOptionFunctionName.Contains(TEXT(".")))
	{
		FString ClassName;
		FString FunctionName;
		PinOptionFunctionName.Split(TEXT("."), &ClassName, &FunctionName
			, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

		if (const UClass* Class = StaticLoadClass(UObject::StaticClass(), nullptr, *ClassName))
		{
			OwningObject = Class->GetDefaultObject();
			PinOptionFunctionName = FunctionName;
		}
	}
	else
    // 处理蓝图定义的默认对象
	{
		if (const UBlueprint* OwningBlueprint = CallFunctionNode->GetBlueprint())
		{
			if (OwningBlueprint->GeneratedClass)
			{
				OwningObject = OwningBlueprint->GeneratedClass->GetDefaultObject();
			}
		}
	}
	if (!OwningObject)
	{
		return nullptr;
	}

    // 查找要执行的函数
	UFunction* FuncToExec = OwningObject->FindFunction(FName(*PinOptionFunctionName));
	if (!FuncToExec)
	{
		return nullptr;
	}

	// 执行函数，并处理返回的选项名
	TArray<TSharedPtr<FName>> OutOptions;
	TArray<FName> OptionNames;
	OwningObject->ProcessEvent(FuncToExec, &OptionNames);
	for (const FName& OptionName : OptionNames)
	{
		OutOptions.Add(MakeShareable(new FName(OptionName)));
	}
	
    // 创建并返回一个新的节点引脚，用于显示选项列表
	return SNew(SGraphPinNameList, InPin, OutOptions);
}
