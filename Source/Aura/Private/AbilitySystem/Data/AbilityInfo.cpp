// Copyright  Mateusz Iwanek


#include "AbilitySystem/Data/AbilityInfo.h"

#include "InterchangeResult.h"
#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	
	for (const FAuraAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		//UE_LOG(LogTemp, Error, TEXT("Ability info not found"));
		UE_LOG(LogAura,Warning,TEXT("Can't find info fro AbilityTag [%s] on AbilityInfo[%s]"),
			*AbilityTag.ToString(), *GetNameSafe(this)); //LogAura custom AuraLogChannel.h
	}
	return FAuraAbilityInfo();
}
