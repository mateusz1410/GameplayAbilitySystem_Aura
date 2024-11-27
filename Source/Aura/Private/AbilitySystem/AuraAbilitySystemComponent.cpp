// Copyright  Mateusz Iwanek


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet() // make sure  ASC is created is valid
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied); //AddUObject so dynamic delegate, no AddDyanmic
}

//call when GE was applied
void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);//get tags from effecct

	for (const FGameplayTag& Tag : TagContainer)
	{
		//TODO: Broadcast the tag to the WidgetController
		const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString()); // Tag.GetTagName() or 
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
	}


	//GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect applied!"));
}
