// Copyright  Mateusz Iwanek


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"


FString UAuraFireBolt::GetDescription(int32 Level)
{
	const float Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(

		// Title
		"<Title>FIRE BOLT</>\n\n"

		// Level
		"<Small>Level: </><Level>%d</>\n"

		// ManaCost
		"<Small>Mana cost: </><ManaCost>%.1f</>\n" // %.1f only one decimal 0.7

		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		
		"<Default>Launched a bolt of fire, "
		"exploding on impact and dealing: </>"

		// Damage
		"<Damage>%.1f</><Default> fire damage with a chance to burn</>"),

		//Values
		Level,
		ManaCost,
		Cooldown,
		Damage);
	}
	else
	{
		return FString::Printf(TEXT(

		// Title
		"<Title>FIRE BOLT</>\n\n"

		// Level
		"<Small>Level: </><Level>%d</>\n"

		// ManaCost
		"<Small>Mana cost: </><ManaCost>%.1f</>\n" // %.1f only one decimal 0.7

		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// Number of FireBolts
		"<Default>Launched %d bolts of fire, exploding on impact and dealing: </>"

		// Damage
		"<Damage>%.1f</><Default> fire damage with a chance to burn</>"),

		//Values
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level,NumProjectiles),
		Damage);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const float Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(

	// Title
	"<Title>NEXT LEVEL</>\n\n"

	// Level
	"<Small>Level: </><Level>%d</>\n"

	// ManaCost
	"<Small>Mana cost: </><ManaCost>%.1f</>\n" // %.1f only one decimal 0.7

	// Cooldown
	"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

	// Number of FireBolts
	"<Default>Launched %d bolts of fire, exploding on impact and dealing: </>"

	// Damage
	"<Damage>%.1f</><Default> fire damage with a chance to burn</>"),

	//Values
	Level,
	ManaCost,
	Cooldown,
	FMath::Min(Level,NumProjectiles),
	Damage);
}