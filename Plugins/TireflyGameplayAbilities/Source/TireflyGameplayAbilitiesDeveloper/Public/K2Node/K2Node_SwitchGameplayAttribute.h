// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_Switch.h"
#include "EdGraph/EdGraphPin.h"
#include "AttributeSet.h"
#include "K2Node_SwitchGameplayAttribute.generated.h"


// Switch on Gameplay Attribute node
// 游戏属性开关节点
UCLASS(MinimalAPI)
class UK2Node_SwitchGameplayAttribute : public UK2Node_Switch
{
	GENERATED_BODY()

#pragma region K2Node_Switch

public:
	UK2Node_SwitchGameplayAttribute(const FObjectInitializer& ObjectInitializer);

	// 向开关节点添加一个新的执行引脚
	// Adds a new execution pin to a switch node
	virtual void AddPinToSwitchNode() override;

	// 获取一个唯一的引脚名称，是序列中的下一个名称
	// Gets a unique pin name, the next in the sequence
	virtual FName GetUniquePinName() override;

	// 从子类的模式中获取引脚类型
	// Gets the pin type from the schema for the subclass
	virtual FEdGraphPinType GetPinType() const override;

	virtual FEdGraphPinType GetInnerCaseType() const override;
	virtual FName GetPinNameGivenIndex(int32 Index) const override;

protected:
	virtual void CreateFunctionPin() override;
	virtual void CreateSelectionPin() override;
	virtual void CreateCasePins() override;
	virtual void RemovePin(UEdGraphPin* TargetPin) override;

#pragma endregion


#pragma region PinProperty

public:
	UPROPERTY(EditAnywhere, Category = PinOptions)
	TArray<FGameplayAttribute> PinAttributes;

	UPROPERTY()
	TArray<FName> PinNames;

#pragma endregion


#pragma region UK2Node

public:
	// 返回节点的属性是否在蓝图详情面板中显示
	// Return whether the node's properties display in the blueprint details panel
	virtual bool ShouldShowNodeProperties() const override { return true; }

	// 替代 GetMenuEntries() 的方法。覆盖此方法以添加与子类类型相关的特定 UBlueprintNodeSpawners。
	// 这是一种可扩展的方式，用于新节点和游戏模块节点将自己添加到上下文菜单中。
	// @param ActionListOut	用于填充新生成器的列表。
	// 
	// Replacement for GetMenuEntries(). Override to add specific UBlueprintNodeSpawners pertaining to the sub-class type.
	// Serves as an extensible way for new nodes, and game module nodes to add themselves to context menus.
	// @param ActionListOut	The list to be populated with new spawners.
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;

#pragma endregion


#pragma region UEdGraphNode

public:
	// 获取当鼠标悬停在节点上时显示的工具提示信息
	// Gets the tooltip to display when over the node
	virtual FText GetTooltipText() const override;

	// 获取此节点的名称，显示在标题栏中
	// Gets the name of this node, shown in title bar
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

#pragma endregion

	
#pragma region UObject

public:
	// 当此对象上的属性在外部被修改时调用
	// @param PropertyChangedEvent 被修改的属性
	// 
	// Called when a property on this object has been modified externally
	// @param PropertyChangedEvent the property that was modified
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	// 在加载对象后立即执行任何特定于对象的清理工作。
	// 对于新创建的对象不会调用此函数，默认情况下始终在游戏线程上执行。
	// 
	// Do any object-specific cleanup required immediately after loading an object.
	// This is not called for newly-created objects, and by default will always execute on the game thread.
	virtual void PostLoad() override;

#pragma endregion
};
