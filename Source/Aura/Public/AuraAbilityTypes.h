#pragma once //header can't be included more than once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h" //GENERATED_BODY()

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
public:

	bool IsCriticalHit() const {return bIsCriticalHit;}
	bool IsBlockedHit() const {return bIsBlockedHit;}

	void SetCriticalHit(bool bInIsCriticalHit) {bIsCriticalHit = bInIsCriticalHit;}
	void SetBlockedHit(bool bInIsBlockedHit) {bIsBlockedHit = bInIsBlockedHit;}

	//---------------- REQ ---------
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	//-------------------
	
protected:

	UPROPERTY()
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;
	
private:
	
};