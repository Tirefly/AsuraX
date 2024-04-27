// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_GetAbilityParameter.generated.h"


UENUM(BlueprintType)
enum class ETireflyAbilityAssetSource : uint8
{
	AbilityClass		UMETA(ToolTip = "通过GameplayAbility的类加载AbilityAsset"),
	AbilityId			UMETA(ToolTip = "通过GameplayAbility的资产Id加载AbilityAsset"),
	AssetRef			UMETA(ToolTip = "直接设置AbilityAsset的硬引用"),
	AssetPak			UMETA(ToolTip = "通过AbilityAsset的PackageName获取AbilityAsset"),
};


class UTireflyGameplayAbilityAsset;


// Get Gameplay Ability Parameter
// GameplayAbilityGraph 专用的获取能力参数节点
UCLASS()
class TIREFLYGAMEPLAYABILITIESDEVELOPER_API UK2Node_GetAbilityParameter : public UK2Node
{
	GENERATED_BODY()

#pragma region K2Node

public:
	// 覆写来设置节点的标题
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	// 覆写来设置节点的注释提示
	virtual FText GetTooltipText() const override;
	// 覆写来设置节点的菜单类别
	virtual FText GetMenuCategory() const override;
	// 覆写来将节点注册到蓝图菜单中，保证能在蓝图菜单中搜索到该节点
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	// 覆写来设置节点的图标标识和颜色
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	// 标记节点是否是纯节点（没有执行引脚，只有返回值的浅绿色节点）
	virtual bool IsNodePure() const override { return bPureNode; }
	// 标记节点是否可以忽略
	virtual bool IsNodeSafeToIgnore() const override { return true; }
	// 覆写来让节点的属性显示出来
	virtual bool ShouldShowNodeProperties() const override { return true; }

	// 覆写来处理节点的属性更改
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

protected:
	// 设置该节点是否是纯节点
	UPROPERTY(EditAnywhere, Category = "Node Options")
	bool bPureNode = true;

	// 节点要使用的能力资产来源
	UPROPERTY(EditAnywhere, Category = "Node Options")
	ETireflyAbilityAssetSource AbilityAssetSource = ETireflyAbilityAssetSource::AbilityClass;
	
#pragma endregion


#pragma region PinCreation

public:
	// 为节点创建引脚
	virtual void AllocateDefaultPins() override;

	/**
	 * 在重建过程中重新分配管脚；默认情况下忽略旧管脚并调用AllocateDefaultPins()。如果您重写此选项以创建其他管脚
	 * ，您可能需要调用 RestoreSplitPins 来恢复任何已拆分的管脚(例如，拆分成其组件的矢量管脚)
	 */
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;

	// 引脚默认值更改时进行的处理
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	// 引脚连接更改时进行的处理
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	// 创建执行输入引脚
	void CreateExecInputPin();
	// 创建执行输出引脚
	void CreateExecOutputPin();
	// 创建能力资产来源引脚
	void CreateAbilityAssetSourcePin();
	// 创建参数名引脚
	void CreateParameterPin();
	// 创建是否成功输出引脚
	void CreateSuccessPin();
	// 创建结果输出引脚
	void CreateResultPin(UClass* ResultType = nullptr, const FString& ResultName = FString());

	// 重新创建变体结果输出引脚
	void RecreateVariantPins();
	// 内部使用的重新创建变体结果输出引脚
	void RecreateVariantPinsInternal(const UTireflyGameplayAbilityAsset* Asset, const FName& Parameter);

	// 获取执行输入引脚
	UEdGraphPin* GetExecInputPin() const;
	// 获取执行输出引脚
	UEdGraphPin* GetExecOutputPin() const;
	// 获取能力资产来源引脚
	UEdGraphPin* GetAbilityAssetSourcePin() const;
	// 获取参数名引脚
	UEdGraphPin* GetParameterPin() const;
	// 获取是否成功输出引脚
	UEdGraphPin* GetSuccessPin() const;
	// 获取结果输出引脚
	UEdGraphPin* GetResultPin() const;

	// 根据能力资产来源引脚获取能力资产
	UTireflyGameplayAbilityAsset* GetAbilityAsset(UEdGraphPin* Pin = nullptr) const;

protected:
	UPROPERTY(VisibleAnywhere)
	UClass* ResultPinType = nullptr;

#pragma endregion
};
