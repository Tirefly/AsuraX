#pragma once

/*
生成时间：2023/10/27 23:4:1
获取Tag数量：74
*/



#define GAS FGameplayTag::RequestGameplayTag("GAS")//游戏性能力系统Tag
#define GAS_ASSET FGameplayTag::RequestGameplayTag("GAS.Asset")//游戏性能力系统Tag：资产
#define GAS_ASSET_ABILITY FGameplayTag::RequestGameplayTag("GAS.Asset.Ability")//游戏性能力系统Tag：能力资产
#define GAS_ASSET_ABILITY_ACTIVESKILL FGameplayTag::RequestGameplayTag("GAS.Asset.Ability.ActiveSkill")//游戏性能力系统Tag：能力资产-主动技能
#define GAS_ASSET_ABILITY_NORMALATTACK FGameplayTag::RequestGameplayTag("GAS.Asset.Ability.NormalAttack")//游戏性能力系统Tag：能力资产-普通攻击
#define GAS_ASSET_ABILITY_PASSIVESKILL FGameplayTag::RequestGameplayTag("GAS.Asset.Ability.PassiveSkill")//游戏性能力系统Tag：能力资产-被动技能
#define GAS_ASSET_EFFECT FGameplayTag::RequestGameplayTag("GAS.Asset.Effect")//游戏性能力系统Tag：效果资产
#define GAS_ASSET_EFFECT_NEGATIVE FGameplayTag::RequestGameplayTag("GAS.Asset.Effect.Negative")//游戏性能力系统Tag：效果资产-减益Effect
#define GAS_ASSET_EFFECT_ONLYCOMBAT FGameplayTag::RequestGameplayTag("GAS.Asset.Effect.OnlyCombat")//游戏性能力系统Tag：效果资产-仅战斗中存在
#define GAS_ASSET_EFFECT_POSITIVE FGameplayTag::RequestGameplayTag("GAS.Asset.Effect.Positive")//游戏性能力系统Tag：效果资产-增益Effect
#define GAS_BAN FGameplayTag::RequestGameplayTag("GAS.Ban")//游戏性能力系统Tag：禁止单位行为
#define GAS_BAN_ACTIVATEACTIVESKILL FGameplayTag::RequestGameplayTag("GAS.Ban.ActivateActiveSkill")//游戏性能力系统Tag：禁止单位行为-主动释放主动技能
#define GAS_BAN_ACTIVESKILL FGameplayTag::RequestGameplayTag("GAS.Ban.ActiveSkill")//游戏性能力系统Tag：禁止单位行为-释放主动技能
#define GAS_BAN_MOVEMENT FGameplayTag::RequestGameplayTag("GAS.Ban.Movement")//游戏性能力系统Tag：禁止单位行为-移动
#define GAS_BAN_NORMALATTACK FGameplayTag::RequestGameplayTag("GAS.Ban.NormalAttack")//游戏性能力系统Tag：禁止单位行为-普通攻击
#define GAS_COMBATUNITSTATE FGameplayTag::RequestGameplayTag("GAS.CombatUnitState")//游戏性能力系统Tag：战斗单位状态
#define GAS_COMBATUNITSTATE_DEATH FGameplayTag::RequestGameplayTag("GAS.CombatUnitState.Death")//游戏性能力系统Tag：战斗单位状态 -死亡
#define GAS_COMBATUNITSTATE_STUNNED FGameplayTag::RequestGameplayTag("GAS.CombatUnitState.Stunned")//游戏性能力系统Tag：战斗单位状态 -被眩晕
#define GAS_COMBATUNITSTATE_INVINCIBLE FGameplayTag::RequestGameplayTag("GAS.CombatUnitState.Invincible")//游戏性能力系统Tag：战斗单位状态 -无敌
#define GAS_COMBATUNITSTATE_INFINITEGRADE FGameplayTag::RequestGameplayTag("GAS.CombatUnitState.InfiniteGrade")//游戏性能力系统Tag：战斗单位状态-无限升级
#define GAS_COMBATUNITSTATE_MELEENOLIMIT FGameplayTag::RequestGameplayTag("GAS.CombatUnitState.MeleeNoLimit")//游戏性能力系统Tag：战斗单位状态-近战普通攻击无目标限制
#define GAS_COMBATUNITSTATE_BATTLING FGameplayTag::RequestGameplayTag("GAS.CombatUnitState.Battling")//游戏性能力系统Tag：战斗单位状态-战斗中
#define GAS_COOLDOWN FGameplayTag::RequestGameplayTag("GAS.Cooldown")//游戏性能力系统Tag：技能冷却
#define GAS_COOLDOWN_SKILL1 FGameplayTag::RequestGameplayTag("GAS.Cooldown.Skill1")//游戏性能力系统Tag：技能1冷却
#define GAS_COOLDOWN_SKILL2 FGameplayTag::RequestGameplayTag("GAS.Cooldown.Skill2")//游戏性能力系统Tag：技能2冷却
#define GAS_COOLDOWN_SKILL3 FGameplayTag::RequestGameplayTag("GAS.Cooldown.Skill3")//游戏性能力系统Tag：技能3冷却
#define GAS_COOLDOWN_SKILL4 FGameplayTag::RequestGameplayTag("GAS.Cooldown.Skill4")//游戏性能力系统Tag：技能4冷却
#define GAS_COOLDOWN_SKILL5 FGameplayTag::RequestGameplayTag("GAS.Cooldown.Skill5")//游戏性能力系统Tag：技能5冷却
#define GAS_GAMEPLAYEVENT FGameplayTag::RequestGameplayTag("GAS.GameplayEvent")//游戏性能力系统Tag：游戏性事件
#define GAS_GAMEPLAYEVENT_ATTACKGETDODGED FGameplayTag::RequestGameplayTag("GAS.GameplayEvent.AttackGetDodged")//游戏性能力系统Tag：游戏性事件-攻击被闪避
#define GAS_GAMEPLAYEVENT_CRITICAL FGameplayTag::RequestGameplayTag("GAS.GameplayEvent.Critical")//游戏性能力系统Tag：游戏性事件-暴击
#define GAS_GAMEPLAYEVENT_GETCRITICAL FGameplayTag::RequestGameplayTag("GAS.GameplayEvent.GetCritical")//游戏性能力系统Tag：游戏性事件-被暴击
#define GAS_GAMEPLAYEVENT_DEATH FGameplayTag::RequestGameplayTag("GAS.GameplayEvent.Death")//游戏性能力系统Tag：游戏性事件-死亡
#define GAS_GAMEPLAYEVENT_DEFEATTARGET FGameplayTag::RequestGameplayTag("GAS.GameplayEvent.DefeatTarget")//游戏性能力系统Tag：游戏性事件-打败目标
#define GAS_GAMEPLAYEVENT_DODGESUCCESSFULLY FGameplayTag::RequestGameplayTag("GAS.GameplayEvent.DodgeSuccessfully")//游戏性能力系统Tag：游戏性事件-闪避成功
#define GAS_GAMEPLAYEVENT_HITTARGET FGameplayTag::RequestGameplayTag("GAS.GameplayEvent.HitTarget")//游戏性能力系统Tag：游戏性事件-攻击命中目标
#define GAS_GAMEPLAYEVENT_GETHIT FGameplayTag::RequestGameplayTag("GAS.GameplayEvent.GetHit")//游戏性能力系统Tag：游戏性事件-被攻击命中
#define GAS_GAMEPLAYEVENT_HITCHECK FGameplayTag::RequestGameplayTag("GAS.GameplayEvent.HitCheck")//游戏性能力系统Tag：游戏性事件-执行攻击检测
#define GAS_GAMEPLAYEVENT_NORMALATTACK FGameplayTag::RequestGameplayTag("GAS.GameplayEvent.NormalAttack")//游戏性能力系统Tag：游戏性事件-普通攻击
#define GAS_GAMEPLAYEVENT_REVIVE FGameplayTag::RequestGameplayTag("GAS.GameplayEvent.Revive")//游戏性能力系统Tag：游戏性事件-复活
#define GAS_SETBYCALLER FGameplayTag::RequestGameplayTag("GAS.SetByCaller")//游戏性能力系统Tag：通过调用设置值
#define GAS_SETBYCALLER_DURATION FGameplayTag::RequestGameplayTag("GAS.SetByCaller.Duration")//游戏性能力系统Tag：游戏性能力系统Tag：通过调用设置值-持续时间
#define GAS_SETBYCALLER_DAMAGE FGameplayTag::RequestGameplayTag("GAS.SetByCaller.Damage")//游戏性能力系统Tag：游戏性能力系统Tag：通过调用设置值-伤害
#define GAS_SETBYCALLER_ATTRIBUTEINITIALIZER FGameplayTag::RequestGameplayTag("GAS.SetByCaller.AttributeInitializer")//游戏性能力系统Tag：通过调用设置值-属性初始化
#define GAS_SETBYCALLER_PHYSIQUE FGameplayTag::RequestGameplayTag("GAS.SetByCaller.Physique")//游戏性能力系统Tag：通过调用设置值-体魄
#define GAS_SETBYCALLER_AGILITY FGameplayTag::RequestGameplayTag("GAS.SetByCaller.Agility")//游戏性能力系统Tag：通过调用设置值-身法
#define GAS_SETBYCALLER_WISDOM FGameplayTag::RequestGameplayTag("GAS.SetByCaller.Wisdom")//游戏性能力系统Tag：通过调用设置值-灵智
#define GAS_SETBYCALLER_MAXHEALTH FGameplayTag::RequestGameplayTag("GAS.SetByCaller.MaxHealth")//游戏性能力系统Tag：通过调用设置值-最大生命值
#define GAS_SETBYCALLER_CURRENTHEALTH FGameplayTag::RequestGameplayTag("GAS.SetByCaller.CurrentHealth")//游戏性能力系统Tag：通过调用设置值-当前生命值
#define GAS_SETBYCALLER_HEALTHRECOVERYRATE FGameplayTag::RequestGameplayTag("GAS.SetByCaller.HealthRecoveryRate")//游戏性能力系统Tag：通过调用设置值-生命值恢复速度
#define GAS_SETBYCALLER_DEFENSIVEFORCE FGameplayTag::RequestGameplayTag("GAS.SetByCaller.DefensiveForce")//游戏性能力系统Tag：通过调用设置值-防御力
#define GAS_SETBYCALLER_ATTACKSPEED FGameplayTag::RequestGameplayTag("GAS.SetByCaller.AttackSpeed")//游戏性能力系统Tag：通过调用设置值-攻击速度
#define GAS_SETBYCALLER_MANARECOVERYOFATTACK FGameplayTag::RequestGameplayTag("GAS.SetByCaller.ManaRecoveryOfAttack")//游戏性能力系统Tag：通过调用设置值-攻击恢复法力量
#define GAS_SETBYCALLER_SKILLINTENSITY FGameplayTag::RequestGameplayTag("GAS.SetByCaller.SkillIntensity")//游戏性能力系统Tag：通过调用设置值-技能强度
#define GAS_SETBYCALLER_ATTACKFORCE FGameplayTag::RequestGameplayTag("GAS.SetByCaller.AttackForce")//游戏性能力系统Tag：通过调用设置值-攻击力
#define GAS_SETBYCALLER_HEALTHSTEALING FGameplayTag::RequestGameplayTag("GAS.SetByCaller.HealthStealing")//游戏性能力系统Tag：通过调用设置值-生命偷取
#define GAS_SETBYCALLER_MANARECOVERYRATE FGameplayTag::RequestGameplayTag("GAS.SetByCaller.ManaRecoveryRate")//游戏性能力系统Tag：通过调用设置值-法力值恢复速度
#define GAS_SETBYCALLER_EXTRAHEALTH FGameplayTag::RequestGameplayTag("GAS.SetByCaller.Shield")//游戏性能力系统Tag：通过调用设置值-护盾值
#define GAS_SETBYCALLER_CRITICALRATE FGameplayTag::RequestGameplayTag("GAS.SetByCaller.CriticalRate")//游戏性能力系统Tag：通过调用设置值-暴击率
#define GAS_SETBYCALLER_CRITICALREDUCEDRATE FGameplayTag::RequestGameplayTag("GAS.SetByCaller.CriticalReducedRate")//游戏性能力系统Tag：通过调用设置值-暴击减免率
#define GAS_SETBYCALLER_CRITICALENHANCERATE FGameplayTag::RequestGameplayTag("GAS.SetByCaller.CriticalEnhanceRate")//游戏性能力系统Tag：通过调用设置值-暴击伤害加成
#define GAS_SETBYCALLER_DAMAGEBONUS FGameplayTag::RequestGameplayTag("GAS.SetByCaller.DamageBonus")//游戏性能力系统Tag：通过调用设置值-伤害加成
#define GAS_SETBYCALLER_DAMAGEREDUCTION FGameplayTag::RequestGameplayTag("GAS.SetByCaller.DamageReduction")//游戏性能力系统Tag：通过调用设置值-伤害减免
#define GAS_SETBYCALLER_HITRATE FGameplayTag::RequestGameplayTag("GAS.SetByCaller.HitRate")//游戏性能力系统Tag：通过调用设置值-命中率
#define GAS_SETBYCALLER_DODGERATE FGameplayTag::RequestGameplayTag("GAS.SetByCaller.DodgeRate")//游戏性能力系统Tag：通过调用设置值-闪避率
#define GAS_SETBYCALLER_SIZE FGameplayTag::RequestGameplayTag("GAS.SetByCaller.Size")//游戏性能力系统Tag：通过调用设置值-体型
#define GAS_SETBYCALLER_MAXMANA FGameplayTag::RequestGameplayTag("GAS.SetByCaller.MaxMana")//游戏性能力系统Tag：通过调用设置值-最大法力值
#define GAS_SETBYCALLER_CURRENTMANA FGameplayTag::RequestGameplayTag("GAS.SetByCaller.CurrentMana")//游戏性能力系统Tag：通过调用设置值-当前法力值
#define GAS_SETBYCALLER_ATTACKTARGETSUM FGameplayTag::RequestGameplayTag("GAS.SetByCaller.AttackTargetSum")//游戏性能力系统Tag：通过调用设置值-攻击目标数量
#define GAS_SETBYCALLER_ATTACKRANGE FGameplayTag::RequestGameplayTag("GAS.SetByCaller.AttackRange")//游戏性能力系统Tag：通过调用设置值-攻击范围
#define GAS_SETBYCALLER_MOVEMENTSPEED FGameplayTag::RequestGameplayTag("GAS.SetByCaller.MovementSpeed")//游戏性能力系统Tag：通过调用设置值-移动速度
#define GAS_SETBYCALLER_SKILLCANCRITICAL FGameplayTag::RequestGameplayTag("GAS.SetByCaller.SkillCanCritical")//游戏性能力系统Tag：通过调用设置值-技能可以暴击
#define GAS_SETBYCALLER_SKILLCRITICALRATE FGameplayTag::RequestGameplayTag("GAS.SetByCaller.SkillCriticalRate")//游戏性能力系统Tag：通过调用设置值-技能携带暴击率
