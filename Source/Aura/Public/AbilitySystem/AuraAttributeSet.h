// Copyright  Mateusz Iwanek

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h" //need for macro
#include "AuraAttributeSet.generated.h"


 #define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

	/*
	*static FGameplayAttribute UMyHealthSet::GetHealthAttribute();
	*FORCEINLINE float UMyHealthSet::GetHealth() const;
	*FORCEINLINE void UMyHealthSet::SetHealth(float NewVal);
	*FORCEINLINE void UMyHealthSet::InitHealth(float NewVal);
	*/
class UAbilitySystemComponent;
class AActor;
class AController;
struct FGameplayEffectContextHandle;
class ACharacter;

USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {};
	FEffectProperties(FGameplayEffectContextHandle InEffectContextHandle, UAbilitySystemComponent* InSourceASC,
		AActor* InSourceAvatarActor, AController* InSourceController, ACharacter* InSourceCharacter, UAbilitySystemComponent* InTargetASC,
		AActor* InTargetAvatarActor, AController* InTargetController, ACharacter* InTargetCharacter)
	: EffectContextHandle(InEffectContextHandle), SourceASC(InSourceASC), SourceAvatarActor (InSourceAvatarActor), SourceController(InSourceController),
		SourceCharacter (InSourceCharacter), TargetASC(InTargetASC), TargetAvatarActor(InTargetAvatarActor),
		TargetController(InTargetController), TargetCharacter(InTargetCharacter){};

	UPROPERTY()
	FGameplayEffectContextHandle EffectContextHandle;

//-----------------------------------
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;


	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

//-----------------------------------

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;

};


/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAuraAttributeSet();

	// no need only .cpp required (in ViusalStudio)
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override; 

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override; //clamp before set

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override; //clamp after effect set, PostGameplayEffectExecute all inf about effect

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health); //macro add functions on Top CTRL+G 10


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attribute")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attribute")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);


	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

private:

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const; //OutProps
};
