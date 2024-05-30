// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireflyAbilitySystemTypes.h"
#include "FireflyGridBase.h"
#include "Engine/DataTable.h"
#include "InstancedStruct.h"
#include "CoreGameplayTypes.generated.h"


class ULegendGameplayAbilityAsset;
class ACombatProjectile;
class ACombatDomain;
class UFireflyGameplayAbility;
class UFireflyGameplayEffect;
class ACombatUnitBase;
class ACombatUnit_Enemy;
class UNiagaraSystem;
class UTexture2D;


#pragma region Synergy

/** 羁绊所属的体系 */
UENUM(BlueprintType)
enum class ESynergyClanType : uint8
{
	None						UMETA(DisplayName = "无"),
	JourneyToTheWest			UMETA(DisplayName = "西游记"),
	Netherworld					UMETA(DisplayName = "幽冥界"),
	ThreeKingdomShu				UMETA(DisplayName = "三国·蜀"),
	ThreeKingdomWei				UMETA(DisplayName = "三国·魏"),
	ThreeKingdomWu				UMETA(DisplayName = "三国·吴"),
	EndOfTheHanDynasty			UMETA(DisplayName = "汉末群雄")
};

/** 羁绊的技能系统数据 */
USTRUCT(BlueprintType)
struct FSynergyAbilitySystemData
{
	GENERATED_BODY()

public:
	// 战斗单位的技能能力数据
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TArray<ULegendGameplayAbilityAsset*> SkillConfigs;
	
};

#pragma endregion


#pragma region CombatUnit

#pragma region CombatUnitEnums

/** 战斗单位的类型 */
UENUM(BlueprintType)
enum class ELegendCombatUnitType : uint8
{
	None		UMETA(Hidden),
	Hero		UMETA(DisplayName = "英雄"),
	Minion		UMETA(DisplayName = "小兵"),
	Elite		UMETA(DisplayName = "精英"),
	Boss		UMETA(DisplayName = "首领"),
	Summon		UMETA(DisplayName = "召唤物")
};

/** 战斗单位的主属性 */
UENUM(BlueprintType)
enum class EMainAttributeType : uint8
{
	Physique = 0	UMETA(DisplayName = "体魄"),
	Agility = 1		UMETA(DisplayName = "身法"),
	Wisdom = 2		UMETA(DisplayName = "灵智")
};

/** 英雄单位的品阶 */
UENUM(BlueprintType)
enum class EHeroRarity : uint8
{
	Blue = 0	UMETA(DisplayName = "品阶：蓝"),
	Purple = 1	UMETA(DisplayName = "品阶：紫"),
	Gold = 2	UMETA(DisplayName = "品阶：金"),
	Max			UMETA(Hidden),
};

#pragma endregion


#pragma region CombatUnitBuilder

/** 战斗单位构建器数据 */
USTRUCT(BlueprintType)
struct LEGENDSTD_API FCombatUnitBuilderData
{
	GENERATED_BODY()

public:
	/** 该单位的名称 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText CombatUnitName = FText();

	/** 该单位的描述 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText CombatUnitDescription = FText();

	/** 该单位的类 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<ACombatUnitBase> CombatUnitClass;

	/** 该单位的类型 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ELegendCombatUnitType CombatUnitType = ELegendCombatUnitType::None;

	/** 该单位的主属性类型 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EMainAttributeType MainAttributeType = EMainAttributeType::Physique;

	/** 该单位所有相关的羁绊ID */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FName> SynergyIDs;

	/** 该单位的所属的羁绊体系 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ESynergyClanType SynergyClan = ESynergyClanType::None;

	/** 该单位的默认皮肤图片 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UTexture2D* DefaultSkinImage = nullptr;

	/** 该单位的其他皮肤图片 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<FName, UTexture2D*> SkinImages;
};

/** 英雄单位核心数据表头 */
USTRUCT(BlueprintType)
struct LEGENDSTD_API FHeroBuilderData : public FCombatUnitBuilderData
{
	GENERATED_BODY()

public:
	/** 该英雄单位的品阶 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EHeroRarity Rarity = EHeroRarity::Blue;

	FHeroBuilderData()
	{
		CombatUnitType = ELegendCombatUnitType::Hero;
	}
};

/** 敌人单位核心数据表头 */
USTRUCT(BlueprintType)
struct LEGENDSTD_API FEnemyBuilderData : public FCombatUnitBuilderData
{
	GENERATED_BODY()

public:
	FEnemyBuilderData()
	{
		CombatUnitType = ELegendCombatUnitType::Minion;
	}
};

/** 召唤单位核心数据表头 */
USTRUCT(BlueprintType)
struct LEGENDSTD_API FSummonBuilderData : public FCombatUnitBuilderData
{
	GENERATED_BODY()

public:
	FSummonBuilderData()
	{
		CombatUnitType = ELegendCombatUnitType::Summon;
	}
};

#pragma endregion


#pragma region NumericalData

// 战斗单位的属性值数据表行
USTRUCT()
struct LEGENDSTD_API FLegendCombatUnitAttributeTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 体魄
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = BaseAttribute)
	float Physique = 0.f;

	// 最大生命值
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = DerivedAttribute)
	float MaxHealth = 0.f;

	// 生命值恢复速度
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = DerivedAttribute)
	float HealthRecoveryRate = 0.f;

	// 身法
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = BaseAttribute)
	float Agility = 0.f;

	// 防御力
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = DerivedAttribute)
	float DefensiveForce = 0.f;

	// 攻击速度
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = DerivedAttribute)
	float AttackSpeed = 0.f;

	// 灵智
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = BaseAttribute)
	float Wisdom = 0.f;

	// 攻击恢复法力量
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = DerivedAttribute)
	float ManaRecoveryOfAttack = 0.f;

	// 技能强度
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = DerivedAttribute)
	float SkillIntensity = 0.f;

	// 攻击力
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = DerivedAttribute)
	float AttackForce = 0.f;

	// 最大法力值
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = ConstantAttribute)
	float MaxMana = 0.f;

	// 攻击范围
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = ConstantAttribute)
	float AttackRange = 0.f;

	// 攻击目标数量
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = ConstantAttribute)
	float AttackTargetSum = 0.f;

	// 移动速度
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = ConstantAttribute)
	float MovementSpeed = 0.f;
};

#pragma endregion


#pragma region CombatUnitSkin

/** 战斗单位的皮肤信息 */
USTRUCT(BlueprintType)
struct LEGENDSTD_API FLegendCombatUnitSkinData
{
	GENERATED_BODY()

public:
	/** 躯干皮肤材质 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UMaterialInterface* MainBodySkin = nullptr;

	/** 右手武器模型 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UStaticMesh* WeaponRightSkin = nullptr;

	/** 左手武器模型 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UStaticMesh* WeaponLeftSkin = nullptr;

	/** 头部冠饰模型 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UStaticMesh* HeaddressSkin = nullptr;
};

#pragma endregion

#pragma endregion


#pragma region CombatExtra

// 战斗生成物的外观数据
USTRUCT(BlueprintType)
struct FCombatExtraSkin
{
	GENERATED_BODY()

public:
	/** 战斗生成物的模型 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UStaticMesh* ExtraMesh = nullptr;

	/** 战斗生成物的材质 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UMaterialInterface* ExtraMeshSkin = nullptr;

	/** 战斗生成物的特效 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UNiagaraSystem* ExtraNS = nullptr;

	FCombatExtraSkin() {}

	FCombatExtraSkin(const FCombatExtraSkin& Other)
		: ExtraMesh(Other.ExtraMesh)
		, ExtraMeshSkin(Other.ExtraMeshSkin)
		, ExtraNS(Other.ExtraNS)
	{}

	FCombatExtraSkin(UStaticMesh* InMesh
		, UMaterialInterface* InSkin, UNiagaraSystem* InNS)
		: ExtraMesh(InMesh)
		, ExtraMeshSkin(InSkin)
		, ExtraNS(InNS)
	{}

	bool operator==(const FCombatExtraSkin& Other) const
	{
		return ExtraMesh == Other.ExtraMesh
			&& ExtraMeshSkin == Other.ExtraMeshSkin
			&& ExtraNS == Other.ExtraNS;
	}
};


#pragma region CombatProjectile

// 战斗投射物的效果结构
USTRUCT(BlueprintType)
struct FCombatProjectileEffect
{
	GENERATED_BODY()

public:
	/** 抛射物的特效 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGameplayEffectSpecHandle> EffectsToTarget;

	FCombatProjectileEffect()
	{
		EffectsToTarget = TArray<FGameplayEffectSpecHandle>{};
	}

	FCombatProjectileEffect(const FCombatProjectileEffect& Other)
	{
		EffectsToTarget = Other.EffectsToTarget;
	}

	FCombatProjectileEffect(const TArray<FGameplayEffectSpecHandle>& InEffects)
	{
		EffectsToTarget = InEffects;
	}

	bool operator==(const FCombatProjectileEffect& Other) const
	{
		return EffectsToTarget == Other.EffectsToTarget;
	}
};

#pragma endregion


#pragma region CombatDomain

// 战斗领域的实时结构
USTRUCT(BlueprintType)
struct FCombatDomainEffect
{
	GENERATED_BODY()

public:
	// 领域会施加给敌方单位的效果（敌方单位进入领域时）
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGameplayEffectSpecHandle> EffectsToEnemiesOnEnter = TArray<FGameplayEffectSpecHandle>{};

	// 领域会施加给敌方单位的效果（敌方单位离开领域时）
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGameplayEffectSpecHandle> EffectsToEnemiesOnExit = TArray<FGameplayEffectSpecHandle>{};

	// 领域会施加给敌方单位的效果（领域将被销毁时）
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGameplayEffectSpecHandle> EffectsToEnemiesOnDestruction = TArray<FGameplayEffectSpecHandle>{};

	// 领域会施加给友方单位的效果（友方单位进入领域时）
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGameplayEffectSpecHandle> EffectsToAlliesOnEnter = TArray<FGameplayEffectSpecHandle>{};

	// 领域会施加给友方单位的效果（友方单位离开领域时）
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGameplayEffectSpecHandle> EffectsToAlliesOnExit = TArray<FGameplayEffectSpecHandle>{};

	// 领域会施加给友方单位的效果（领域将被销毁时）
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGameplayEffectSpecHandle> EffectsToAlliesOnDestruction = TArray<FGameplayEffectSpecHandle>{};

	FCombatDomainEffect() {}

	FCombatDomainEffect(const FCombatDomainEffect& Other)
	{
		EffectsToEnemiesOnEnter = Other.EffectsToEnemiesOnEnter;
		EffectsToEnemiesOnExit = Other.EffectsToEnemiesOnExit;
		EffectsToEnemiesOnDestruction = Other.EffectsToEnemiesOnDestruction;
		EffectsToAlliesOnEnter = Other.EffectsToAlliesOnEnter;
		EffectsToAlliesOnExit = Other.EffectsToAlliesOnExit;
		EffectsToAlliesOnDestruction = Other.EffectsToAlliesOnDestruction;
	}

	FCombatDomainEffect(
		const TArray<FGameplayEffectSpecHandle>& ToEnemiesOnEnter
		, const TArray<FGameplayEffectSpecHandle>& ToEnemiesOnExit
		, const TArray<FGameplayEffectSpecHandle>& ToEnemiesOnCheck
		, const TArray<FGameplayEffectSpecHandle>& ToEnemiesOnDestruction
		, const TArray<FGameplayEffectSpecHandle>& ToAlliesOnEnter
		, const TArray<FGameplayEffectSpecHandle>& ToAlliesOnExit
		, const TArray<FGameplayEffectSpecHandle>& ToAlliesOnCheck
		, const TArray<FGameplayEffectSpecHandle>& ToAlliesOnDestruction)
	{
		EffectsToEnemiesOnEnter = ToEnemiesOnEnter;
		EffectsToEnemiesOnExit = ToEnemiesOnExit;
		EffectsToEnemiesOnDestruction = ToEnemiesOnDestruction;
		EffectsToAlliesOnEnter = ToAlliesOnEnter;
		EffectsToAlliesOnExit = ToAlliesOnExit;
		EffectsToAlliesOnDestruction = ToAlliesOnDestruction;
	}

	bool operator==(const FCombatDomainEffect& Other) const
	{
		return EffectsToEnemiesOnEnter == Other.EffectsToEnemiesOnEnter
			&& EffectsToEnemiesOnExit == Other.EffectsToEnemiesOnExit
			&& EffectsToEnemiesOnDestruction == Other.EffectsToEnemiesOnDestruction
			&& EffectsToAlliesOnEnter == Other.EffectsToAlliesOnEnter
			&& EffectsToAlliesOnExit == Other.EffectsToAlliesOnExit
			&& EffectsToAlliesOnDestruction == Other.EffectsToAlliesOnDestruction;
	}
};

#pragma endregion

#pragma endregion


#pragma region EnemyFormat

// 敌人阵型数据表行
USTRUCT(BlueprintType)
struct FLegendEnemyFormatTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 下波间隔时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "下波间隔时间"))
	float NextFormatInterval = 1.f;

	// 阵型首领ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "阵型首领ID"))
	FName BossEnemyID = NAME_None;

	// 阵型敌人棋子
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "阵型敌人棋子"))
	TMap<FFireflyGridCoordinate, FName> FormatEnemies;
};

// 敌人阵型实时数据
USTRUCT(BlueprintType)
struct FLegendEnemyFormatRuntime
{
	GENERATED_BODY()

	// 阵型敌人棋子
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "阵型敌人棋子"))
	TMap<FFireflyGridCoordinate, FName> FormatEnemies;

	// 这波敌人的等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "阵内敌人等级"))
	int32 FormatGrade = 1;

	// 下波间隔时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "下波间隔时间"))
	float NextFormatInterval = 1.f;
};

#pragma endregion


#pragma region Roguelike

// 肉鸽房间类型
UENUM(BlueprintType)
enum class ERogueRoomType : uint8
{
	None	UMETA(Hidden),
	Minion	UMETA(DisplayName = "小兵怪房"),
	Elite	UMETA(DisplayName = "精英怪房"),
	Boss	UMETA(DisplayName = "首领怪房"),
	Event	UMETA(DisplayName = "事件房间"),
	Shop	UMETA(DisplayName = "商店房间")
};

// 战斗房间
USTRUCT(BlueprintType)
struct FCombatRoomEnemyWave
{
	GENERATED_BODY()

public:
	// 小兵出阵波数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinionWaveNum = 0;

	// 精英出阵波数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EliteWaveNum = 0;

	// 首领出阵波数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BossWaveNum = 0;	
};

// 肉鸽房间实时数据
USTRUCT(BlueprintType)
struct FRogueRoomRuntime
{
	GENERATED_BODY()

public:
	// 房间类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERogueRoomType RoomType = ERogueRoomType::None;

	// 房间层级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RoomHierarchy = 1;

	// 房间层级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RoomGrade = 1;
};

// 肉鸽房间实时数据：战斗房间
USTRUCT(BlueprintType)
struct FRogueRoomRuntime_Combat : public FRogueRoomRuntime
{
	GENERATED_BODY()

public:
	// 该房间敌人的阵型数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> EnemyFormats;
};

// 肉鸽房间实时数据：事件房间
USTRUCT(BlueprintType)
struct FRogueRoomRuntime_Event : public FRogueRoomRuntime
{
	GENERATED_BODY()

public:
};

// 肉鸽房间实时数据：商店房间
USTRUCT(BlueprintType)
struct FRogueRoomRuntime_Shop : public FRogueRoomRuntime
{
	GENERATED_BODY()

public:
};

#pragma endregion


#pragma region GameMode

/** 游戏阶段 */
UENUM(BlueprintType)
enum class ELegendGameStage : uint8
{
	Loading		UMETA(DisplayName = "游戏加载中"),
	Waiting		UMETA(DisplayName = "准备战斗阶段"),
	InBattle	UMETA(DisplayName = "战斗阶段"),
	GameEnd		UMETA(DisplayName = "游戏结束")
};

#pragma endregion


#pragma region ArenaScene

/** 游戏场景棋格类型 */
UENUM(BlueprintType)
enum class EArenaSceneGridType : uint8
{
	None,
	Auditorium			UMETA(DisplayName = "备战席"),
	PlayerFormation		UMETA(DisplayName = "玩家战区"),
	EnemyFormation		UMETA(DisplayName = "敌人战区"),
	Battlefield			UMETA(DisplayName = "自由战场")
};

#pragma endregion


#pragma region GameplayCueVFX

#pragma region CueVFX

// 战斗VFX数据
USTRUCT(BlueprintType)
struct FLegendCueVFX
{
	GENERATED_BODY()
};

// 战斗VFX数据：Niagara
USTRUCT(BlueprintType)
struct FLegendCombatCueVFX_Niagara
	: public FLegendCueVFX
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> NiagaraCue;
};

// 战斗VFX数据：材质
USTRUCT(BlueprintType)
struct FLegendCombatCueVFX_Material
	: public FLegendCueVFX
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInterface> MaterialCue;
};

#pragma endregion

// 战斗VFX数据表行
USTRUCT(BlueprintType)
struct FLegendGameplayCueTableRowCueVFX
	: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (BaseStruct = "/Script/LegendsTD.LegendCueVFX", ExcludeBaseStruct))
	TMap<FName, FInstancedStruct> GameplayCueVFXs;
};

#pragma endregion