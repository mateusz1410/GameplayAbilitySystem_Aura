// Copyright  Mateusz Iwanek


#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "UObject/Object.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "Fonts/UnicodeBlockRange.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get(); // local ref to AuraGameplayTags


	/*Primary Attributes*/
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	/*Secondary Attributes*/
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	// REPNOTIFY_Always rep even is set to same value, REPNOTIFY_OnChanged when new value is diffrent
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{	
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	//	UE_LOG(LogTemp, Warning, TEXT("Health %f"), NewValue);
	}

	if (Attribute == GetMaxHealthAttribute())
	{

		NewValue = NewValue < 0.f ? 0.f : NewValue;
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	if (Attribute == GetMaxManaAttribute())
	{
		NewValue = NewValue < 0.f ? 0.f : NewValue;
	}

}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{

/*******************************   Source == couser of the effect, Target = target of the effect (owner of this AS, this class)   ********************/

	// get data from FGameplayEffectModCallbackData& Data and put in FEffectProperties& Props

	Props.EffectContextHandle = Data.EffectSpec.GetContext();// const FGameplayEffectContextHandle
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent(); // const UAbilitySystemComponent* ..  EffectContextHandle.GetOriginalIn ... //asc of source


	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.Get())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		
		UE_LOG(LogTemp, Warning, TEXT("Changed Health on %s, Health: %f"),*Props.TargetAvatarActor->GetName(), GetHealth());
	}

	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{

		if(GetMaxHealth()<0.f) SetMaxHealth(0.f);
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{

		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		if (GetMaxMana() < 0.f) SetMaxMana(0.f);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage =  GetIncomingDamage(); // damage to apply
		SetIncomingDamage(0.f);
		
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth()-LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			
			const bool bFatal = NewHealth <= 0.f;
			if (!bFatal)
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effect_HitReact);
				
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer); //HitReaction activate
			}
		}
		
	}

	/* LOG

	UE_LOG(LogTemp, Warning, TEXT("Props EffectContextHandle %s"), *Props.EffectContextHandle.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Props SourceASC %s"), *Props.SourceASC->GetFullName());
	UE_LOG(LogTemp, Warning, TEXT("Props SourceAvatarActor %s"), *Props.SourceAvatarActor->GetFullName());
	UE_LOG(LogTemp, Warning, TEXT("Props SourceCharacter %s"), *Props.SourceCharacter->GetFullName());
	UE_LOG(LogTemp, Warning, TEXT("Props SourceController %s"), *Props.SourceController->GetFullName());

	UE_LOG(LogTemp, Warning, TEXT("Props TargetASC %s"), *Props.TargetASC->GetFullName());
	UE_LOG(LogTemp, Warning, TEXT("Props TargetAvatarActor %s"), *Props.TargetAvatarActor->GetFullName());
	UE_LOG(LogTemp, Warning, TEXT("Props TargetCharacter %s"), *Props.TargetCharacter->GetFullName());
	UE_LOG(LogTemp, Warning, TEXT("Props TargetController %s"), *Props.TargetController->GetFullName());

	*/

	/*
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Health %f"), GetHealth());
		//UE_LOG(LogTemp, Warning, TEXT("Maginitude %f"), Data.EvaluatedData.Magnitude);
	}
	*/
}
#pragma region OnRep functions
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth); //GAS
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

#pragma endregion
