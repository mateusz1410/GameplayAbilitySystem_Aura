// Copyright  Mateusz Iwanek


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AuraAbilityTypes.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "UI/WidgetController/AuraWidgetController.h"
 
UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC= UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
		{
			if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
			{
				AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
				UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
				UAttributeSet* AS = PS->GetAttributeSet();
				const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

				return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
			}
		}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	UCharacterClassInfo*  CharacterClassInfo= GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	/** ApplyPrimaryAttributes to Enemy/CharacteClass*/
	FGameplayEffectContextHandle  PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	/** ApplySecondaryAttributes to Enemy/CharacteClass*/
	FGameplayEffectContextHandle  SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	/** ApplyVitalAttributes to Enemy/CharacteClass*/
	FGameplayEffectContextHandle   VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo*  CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	//CommonAbilities
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass.Get(), 1);// in this game common are base, 1 lvl is ok.
		ASC->GiveAbility(AbilitySpec);
	}
	
	//ClassAbilities
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (const TSubclassOf<UGameplayAbility>& AbilityClass: DefaultInfo.StartupAbilities)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass.Get(), CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
	
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;
	
	return AuraGameMode->CharacterClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	// static_cast -  c++ compiler cast, Cast<>() only on UObject, this is not UObject, no need for dynamic_cast 
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	// static_cast -  c++ compiler cast, Cast<>() only on UObject, this is not UObject, no need for dynamic_cast 
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	 FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	 TArray<FOverlapResult> Overlaps; //OverlapsResaoults
	 if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	 {	
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);

	 	for (FOverlapResult& Overlap : Overlaps)
	 	{
	 		if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
	 		{
	 			OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor())); //Overlap.GetActor()
	 		}
	 	}
	 }
}
