// Copyright  Mateusz Iwanek

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 * 
 * Singleton containing native GameplayTags
 */
struct FAuraGameplayTags
{

public:

	static const FAuraGameplayTags& Get() {return GameplayTags; };
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;


	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	FGameplayTag Attributes_Meta_IncomingXP;
	
	//-----------------------

	//Key
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;

	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_5;
	FGameplayTag InputTag_6;
	FGameplayTag InputTag_7;
	FGameplayTag InputTag_8;
	FGameplayTag InputTag_9;
	FGameplayTag InputTag_0;

	//no key, but next to key, ID
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	//----------------------------

	//Damage Type
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

	//-----------------------
	//works like secondary attribute, base on Primary
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;
	
	//Debuff effect caused by Ability, Fire can cause burn ----------------------
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Physical;

	// info. debuff
	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Frequency;
	FGameplayTag Debuff_Duration;
	//---------------------------------------------------
	FGameplayTag Abilities_None;
	
	// NPC action
	FGameplayTag Abilities_Attack; 
	FGameplayTag Abilities_Summon;

	//Hit anim
	FGameplayTag Abilities_HitReact;

	//Can player use Ability, status
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;

	//Ability type
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None; // hitReact etc.

	//Player attack ability
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Lightning_Electrocute;
	FGameplayTag Cooldown_Fire_FireBolt;
	

	//------------------------------------
	FGameplayTag CombatSocket_Weapon; //with weapon
	FGameplayTag CombatSocket_RightHand;//hand/claw attack
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;
	//--------------------------
	
	FGameplayTag Montage_Attack_1;  // character attack 1, ID
	FGameplayTag Montage_Attack_2;  // character attack 2
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	//-----------------------
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
	//----------------------------
	FGameplayTag Effect_HitReact;

	
private:
	static FAuraGameplayTags GameplayTags;

};