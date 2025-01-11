// Copyright  Mateusz Iwanek


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;
	bool bSearching = true;
	
	while (bSearching)
	{
		//LevelUpInformation[1] = Level 1 Information, LevelUpInformation[0] is empty in DA
		//LevelUpInformation[2] = Level 2 Information
		if (LevelUpInformation.Num() -1 <= Level) return Level; // no more Level in DA

		if (XP >= LevelUpInformation[Level].LevelUpRequirement) // player got more XP than req
		{
			++Level;
		}
		else
		{
			bSearching = false; 
		}
	}
	
	return Level;
}
