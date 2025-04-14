// Copyright  Mateusz Iwanek


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

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
	return FString::Printf(TEXT("<Default>Spell Locked until level: </><Level>%i</>"), Level);
}
