// Copyright  Mateusz Iwanek


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Aura/AuraLogChannels.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet() // make sure  ASC is created is valid
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied); //AddUObject so dynamic delegate, no AddDyanmic

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
		 FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1); // 1 level, with curveTable

		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);

			GiveAbility(AbilitySpec); //give ability 

			//GiveAbilityAndActivateOnce(AbilitySpec); // give and activate
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	//GetActivatableAbilities() // get all abilities that can be activated
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) 
		{
			AbilitySpecInputPressed(AbilitySpec);// set bool InputPressed, call virtual function InputPressed no body, can be override to give some additional effect.
			//AbilitySpecInputReleased(AbilitySpec);//set bool InputPressed, call virtual function InputReleased no body, can be override to give some additional effect.
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}

}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	//GetActivatableAbilities() // get all abilities that can be activated
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	/** block scope*/
	FScopedAbilityListLock ActiveScopeLock(*this); // block scope, prevent issue when ability change, while in for loop, this ASC
	
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) // for each ability 
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec)) //call delegate
		{
			UE_LOG(LogAura,Error,TEXT("Failed to execute delegate in %hs"),__FUNCTION__);// __FUNCTION__  == ForEachAbility, this f. disp. name 
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for ( FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))// return tag contain Abilities
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag")))) // one input tag
		{
			return Tag;
		}
	}
	
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven) //First time false, will be false on client no rep. 
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast(this); 
	}
}

//call when GE was applied
void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);//get tags from effecct

	EffectAssetTags.Broadcast(TagContainer); // let WidgetController no, update ui and effect

	//GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect applied!"));
}
