// Copyright  Mateusz Iwanek

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

/**
*enum classes - enumerator names are local to the enum
*and their values do not implicitly convert to other types (like another enum or int)

Plain enums - where enumerator names are in the same scope as the enum
and their values implicitly convert to integers and other types
 *
 **/

class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist UMETA(DisplayName = "Elementalist"),
	Warrior UMETA(DisplayName = "Warrior"),
	Ranger UMETA(DisplayName = "Ranger")
	
};
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes; // init all PA with one GE, value for each class can be different
	
};
/**
 * 
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset //use in AuraGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults") //set in Editor
	TMap<ECharacterClass,FCharacterClassDefaultInfo> CharacterClassInformation;
	
	//-----------------------
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes; // dependency on Primary is common
	
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes; // mana and HP
	//-----------------------
	
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass) const;
};
