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

    const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get()); // pointer value 
	 // EffectSpecHandle -get- FGameplayEffectSpecHandle ---> .Data -get- TSherePtr<FGameplayEffectSpec> --->.Get() - get- *TSherePtr<FGameplayEffectSpec> ---> *  -get -  ref &
	
}

