// Copyright  Mateusz Iwanek

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1; // amount of Attribute point granted after levelUp

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 1; // skill points
};
/**
 * 
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation; //LevelUpInformation[1] = Level 1 Information, LevelUpInformation[0] is empty in DA

	/** f.e Player got 1000 XP, is on Level 5*/
	int32 FindLevelForXP(int32 XP) const; 
};
