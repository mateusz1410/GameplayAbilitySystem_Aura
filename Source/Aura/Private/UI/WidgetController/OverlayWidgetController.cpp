// Copyright  Mateusz Iwanek


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h" 
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	// init value, call with init value
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth()); // GetAuraAS() parent function.
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());

	// change value, bind to delegate are located in BindCallbacksToDependencies
	// .... 
	///call in  AuraHUd -> InitOverlay() -> GetOverlayWidgetControlle() // <- this create widgetOverlayController

}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	 // all  Aura widget controller have PlayerState
	GetAuraPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);

	GetAuraPS()->OnLevelChangedDelegate.AddLambda( //OnLevelChangedDelegate(int32)
		[this](int32 NewLevel){
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
	});
	
	// change value bind  hp drop, heal etc.

	/* instedOf Lambda
 
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);

		//*******  and *********

			void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
			{
				OnHealthChanged.Broadcast(Data.NewValue);

			}

			void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);

			}

			void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
			{
				OnManaChanged.Broadcast(Data.NewValue);

			}

			void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
	*/

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnHealthChanged.Broadcast(Data.NewValue); } );

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnMaxHealthChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnManaChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnMaxManaChanged.Broadcast(Data.NewValue); });

	if (GetAuraASC())
	{
		GetAuraASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		
		if (GetAuraASC()->bStartupAbilitiesGiven) 
		{
			BroadcastAbilityInfo(); //already given, so init data
		}
		else
		{
			//wait for call and then init 
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}

		// add lambda  == AddUObject(this, function ref )
		GetAuraASC()->EffectAssetTags.AddLambda(
		
			[this](const FGameplayTagContainer& AssetTags) //[this]  -- out of scope GetDataTableRowByTag, FUIWidgetRow, MessageWidgetDataTable. this means this class 
			{

				for (const FGameplayTag& Tag : AssetTags)
				{
					//* "A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False

					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message")); //get Tag Message
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag); //FUIWidgetRow struct use in DT 
						MessageWidgetRowDelegate.Broadcast(*Row);
					}

					//const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString()); // Tag.GetTagName() or 
					//GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
				}
			}  
		); // [cc](function parameter){function body/definition}
	}
	
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT(" Unable to find LevelUpInformation. Please fill out AuraPlayerState Blueprint")); //is set in BP
	
	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num(); // [0] is empty, start from [1]
	
	if (Level <=MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;//f.g 300
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement; //f.g 900

		const int32 DeltaLevelRequirement  = LevelUpRequirement-PreviousLevelUpRequirement; // req for next level 600
		const int32 XPForThisLevel = NewXP-PreviousLevelUpRequirement;// newXP = 450 (all XP), 450-300 = 150

		const float XPBarPercentage = static_cast<float>(XPForThisLevel)/static_cast<float>(DeltaLevelRequirement); // 150/600

		OnXPPercentChangedDelegate.Broadcast(XPBarPercentage); 
	}
	
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty info if PreviousSlot is a valid slot. Broadcast empty/clear (if equipping an already equipped spell)
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	// Broadcast current slot info, new slot
	AbilityInfoDelegate.Broadcast(Info);
}