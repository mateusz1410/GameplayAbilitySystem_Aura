// Copyright  Mateusz Iwanek


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor); // better use this insted of cast
	
	if (TargetASC == nullptr) return;

	checkf(GameplayEffectClass,TEXT("GameplayEffectClass class is null, please set it in EffectActor"))

	FGameplayEffectContextHandle EffectContextHandle =  TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this); // what cose effect

    const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get()); // pointer value 
	 // EffectSpecHandle -get- FGameplayEffectSpecHandle ---> .Data -get- TSherePtr<FGameplayEffectSpec> --->.Get() - get- *TSherePtr<FGameplayEffectSpec> ---> *  -get -  ref &
	
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

		//>Def TObjectPtr<UGameplayEffect> ----> get() gameplay effect default properties


/********************** REMOVE GE  STEP1 -- Marks To remove-- ********************************/
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::EERP_RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC); // if infinite GE and remove onEndOverlap add do map
	}
/*** End STEP1****/

	if (bDestroyOnEffectApplication && !bIsInfinite)
	{
		Destroy();
	}

}

void AAuraEffectActor::ApplyMultipleEffectsToTarget(AActor* TargetActor, TArray<TSubclassOf<UGameplayEffect>> GameplayEffectClassArray)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;

	for (auto GameplayEffect : GameplayEffectClassArray)
	{
		ApplyEffectToTarget(TargetActor, GameplayEffect);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::EEAP_OnOverlap)
	{
		ApplyMultipleEffectsToTarget(TargetActor, InstantGameplayEffectClass); //before ApplyEffectToTarget for testing ApplyMultipleEffectsToTarget
	}


	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::EEAP_OnOverlap)
	{
		ApplyMultipleEffectsToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::EEAP_OnOverlap)
	{
		ApplyMultipleEffectsToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::EEAP_OnEndOverlap)
	{
		ApplyMultipleEffectsToTarget(TargetActor, InstantGameplayEffectClass);
	}
	//EEffectRemovalPolicy  InstantEffectApplicationPolic is no needed remove after application
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::EEAP_OnEndOverlap)
	{
		ApplyMultipleEffectsToTarget(TargetActor, DurationGameplayEffectClass);
	}
	//EEffectRemovalPolicy  DurationEffectApplicationPolicy is no needed, will remove after duration

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::EEAP_OnEndOverlap)
	{
		ApplyMultipleEffectsToTarget(TargetActor, InfiniteGameplayEffectClass);
	}


/******************************* REMOVE GE  STEP2 **************************************************/

	//EEffectRemovalPolicy InfiniteEffectRemovalPolicy
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::EERP_RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		
		if (!IsValid(TargetASC)) return;


		TArray<FActiveGameplayEffectHandle> HandlesToRemove; //local scope

		for (auto HandlePair : ActiveEffectHandles) // auto == TPair<FAbilityGameplayEfectHandle,UAbilitySystemComponent*>  in this loop
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1); //remove gameplay effect, second parameter remove from stack 1
				HandlesToRemove.Add(HandlePair.Key);  //ActiveEffectHandles.Remove(HandlePair.Key); remove from map when loop can crash game
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle); //safe remove from map,

			
		}

	}

}

