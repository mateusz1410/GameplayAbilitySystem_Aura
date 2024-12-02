// Copyright  Mateusz Iwanek


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet() // make sure  ASC is created is valid
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied); //AddUObject so dynamic delegate, no AddDyanmic

	/*
	const FAuraGameplayTags& GameplayTags =  FAuraGameplayTags::Get();

	//GameplayTags.Attributes_Secondary_Armor.ToString();
	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Green, GameplayTags.Attributes_Secondary_Armor.ToString());//-1 all, 1 one on screen
	
	*/

}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbility)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbility)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1); // 1 level
		//GiveAbility(AbilitySpec); //give ability 
		GiveAbilityAndActivateOnce(AbilitySpec); // give and activate
	}

}

//call when GE was applied
void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);//get tags from effecct

	EffectAssetTags.Broadcast(TagContainer); // let WidgetController no, update ui and effect

	//GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect applied!"));
}
