// Copyright  Mateusz Iwanek


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle =  MakeOutgoingGameplayEffectSpec(DamageEffectClass,1.f);//GE
	for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageTypes )
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel()); //CT
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);//GE  magnitude
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));// apply GE
	
}
