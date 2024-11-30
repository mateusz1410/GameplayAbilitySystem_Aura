// Copyright  Mateusz Iwanek


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// definition of how to capture Attribute Vigor
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute(); //store Attribute
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target; // take from Target
	VigorDef.bSnapshot = false; //capture when GE create or when GE is applied. True when created


	RelevantAttributesToCapture.Add(VigorDef); //aray of attributes we want to capture. req
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
/******       Get Current Vigor value           *****/

	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	//-----------------------------------
	float Vigor = 0.f;// out param
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor); // vigor var will have value of the vigor attribute on the target

	Vigor = FMath::Max<float>(Vigor, 0.f); // vigor should be positive

/******       Get Current Level value           *****/

	// get Level of GE Owner, player character
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject()); //GetSourceObject() - get actor owner of ge
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();


/******       Calculate magnitude            *****/
	
	float BaseValue = 80.f;
	return BaseValue + (2.5f * Vigor) + (10.f * PlayerLevel);

	//Player MaxHealth is equal 80, +2.5 MaxHP for every Vigor point, and 10 MaxHP for Level
	// Player gets additional MaxHP when level up and when invest in Vigor attribute
}
