#pragma once //header can't be included more than once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h" //GENERATED_BODY()

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	/** NEED TO SET CONTEXT CLASS IN PROJECT,
	 * add 
	 * AuraAbilitySystemGlobals
	 * override
	 * virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	 *
	 * use in MakeContext()
	*/
public:

	bool IsCriticalHit() const {return bIsCriticalHit;}
	bool IsBlockedHit() const {return bIsBlockedHit;}

	void SetCriticalHit(bool bInIsCriticalHit) {bIsCriticalHit = bInIsCriticalHit;}
	void SetBlockedHit(bool bInIsBlockedHit) {bIsBlockedHit = bInIsBlockedHit;}

	//---------------- REQ ---------
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}
	
	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	//-------------------
	
protected:

	UPROPERTY()
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;
	
};

//REQ, operation on Struct FAuraGameplayEffectContext
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true, //can serialize
		WithCopy = true //can copy
	};
};