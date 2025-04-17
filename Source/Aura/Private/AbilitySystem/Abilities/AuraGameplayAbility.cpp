// Copyright  Mateusz Iwanek


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

/* need to be override*/
FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	// L"Default Ability Name - " literal text
	return FString::Printf(TEXT("<Default>%s </> <Level>%i</>"),
	L"Default Ability Name - Default text to test ability needs to be overridden in child class, just for testing purposes", Level);
}

/* need to be override*/
FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s </> <Level>%i</> \n Cause much more damage"),
	L"Next Level Ability Name - Default text to test ability needs to be overridden in child class, just for testing purposes", Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked</>\n\n<Default>until level: </><Level>%i</>"), Level);
}

float UAuraGameplayAbility::GetManaCost(int32 InLevel) const
{
	float ManaCost = 0.f;
	
	const UGameplayEffect* CostEffect = GetCostGameplayEffect(); // default GetCostGameplayEffect every ability
	if (CostEffect)
	{
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())// if GE cost is Mana
			{
				//if CurveTable or ScalableFloat then it will work and fill ManaCost
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel,ManaCost); // there is many types of magnitude, 
				break;	
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(int32 InLevel) const
{
	const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect(); // default GetCooldownGameplayEffect every ability
	float Cooldown = 0.f;
	
	if (CooldownEffect)
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel,Cooldown);
	}
	
	return Cooldown;
}
