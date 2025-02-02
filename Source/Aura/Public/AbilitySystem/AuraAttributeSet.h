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

// no need // DECLARE_DELEGATE_RetVal(FGameplayAttribute, FAttributeSignature);// outval, Name
// BindStatic()
//FGameplayAttribute Attr = Pair.Value.Execute(); //execute delegate, DECLARE_DELEGATE_RetVal(FGameplayAttribute, FAttributeSignature);// outval, Name

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

#pragma region FuncPtrExplanation
/**********  FunctionPointer is pointer on function returning FGameplayAttribute, *******************/

//TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FunctionPointer; //::FFuncPtr is memeber, 
//TMap<FGameplayTag, TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr> TagsToAttributes; // Old TMap<FGameplayTag, FAttributeSignature> TagsToAttributes; 


//typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr; // //typedef type AliasTypeName;

//raw c++ version of function pointer returning FGameplayAttribute, no params.
// 
// FAttributeFuncPtr == FGameplayAttribute(*)()   // !! ---- same type ------ !!
// 
// declare pointer to function:  typedef ReturnType (* FunctionPointerName)(ParamType1, Paramtype2); ReturnType = FunctionName - assign; ReturnType() -exec;

//---------------------------------------------------------

/*
template<class T>
using TAliasTypeName = typename TArray<T> // template alias;  
*/

// pointer to static function  becouse - TBase STATIC DelegateInstance<T....											
// Variadic param - any quantity, TBaseStaticDelegateInstance<RetValType(ParamTypes...), UserPolicy, VarTypes...>
// 
// == T(*)() -- pointer to function returning T type,
#pragma endregion

template<class T>
using TStaticFunPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr; 

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

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override; // after attribute change (current value), after MMC and Exec_Calc
	
//------------------------------------------------

TMap<FGameplayTag, TStaticFunPtr<FGameplayAttribute()>> TagsToAttributes;// any function returning FGameplayAttribute

//-----------------------------------------------------

	/*  
	*Primary Attributes  
	*/

#pragma region Primary Attributes

	// Initialize by GE

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attribute")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attribute")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attribute")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attribute")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);

#pragma endregion

	/*
	*Secondary Attributes
	*/

#pragma region Secondary Attributes

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attribute")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attribute")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attribute")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attribute")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attribute")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attribute")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attribute")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attribute")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attribute")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

#pragma endregion

	/*  
	*Resistance Attributes  
	*/
#pragma region Resistance Attributes

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "Resistance Attribute")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance, Category = "Resistance Attribute")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneResistance, Category = "Resistance Attribute")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Resistance Attribute")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance);

#pragma endregion Resistance Attributes	

	/*  
	*Vital Attributes
	*/

#pragma region Vital Attributes 
	
	// Initialize in GE

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health); //macro add functions on Top CTRL+G 10


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attribute")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);


#pragma endregion

	
	/*  
	*Meta Attributes
	*/

#pragma region Meta Attributes
	
//Meta only on server, calculation
	
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingXP);
	
#pragma endregion
	
	
	/**Vital Attributes*/

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;


	/**Primary Attributes*/

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;


	/**Secondary Attributes*/

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance)const;
	
	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance)const;
	
	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance)const;

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance)const;

private:

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const; //OutProps
	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const;
	void SendXPEvent(const FEffectProperties& Props);

	bool bTopOffHealth = false;
	bool bTopOffMana = false;
};

