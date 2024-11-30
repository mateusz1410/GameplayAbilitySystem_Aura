// Copyright  Mateusz Iwanek

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:

	UMMC_MaxHealth();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override; //return resault for magnitude

private:

	FGameplayEffectAttributeCaptureDefinition VigorDef; //VigorDeffinition,  MMC is dependent in calculation on Attribute Vigor

};
