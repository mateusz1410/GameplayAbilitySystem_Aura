// Copyright  Mateusz Iwanek


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h" 
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{

	Super::BroadcastInitialValues();

	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	// init value, call with init value
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());

	// change value, bind to delegate are located in BindCallbacksToDependencies
	// .... 
	///call in  AuraHUd -> InitOverlay() -> GetOverlayWidgetControlle() // <- this create widgetOverlayController

}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState); // all  Aura widget controller have PlayerState
	AuraPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);

	AuraPlayerState->OnLevelChangedDelegate.AddLambda( //OnLevelChangedDelegate(int32)
		[this](int32 NewLevel){
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
	});
	
	// change value bind  hp drop, heal etc.
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

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
		AuraAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnHealthChanged.Broadcast(Data.NewValue); } );

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnMaxHealthChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnManaChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnMaxManaChanged.Broadcast(Data.NewValue); });

	if (UAuraAbilitySystemComponent*  AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		
		if (AuraASC->bStartupAbilitiesGiven) 
		{
			OnInitializeStartupAbilities(AuraASC); //already given, so init data
		}
		else
		{
			//wait for call and then init 
			AuraASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}

		// add lambda  == AddUObject(this, function ref )
		AuraASC->EffectAssetTags.AddLambda(
		
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

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent)
{
	//Get information about all given abilities, look up their Ability Info, and broadcast it to widget
	if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;

	// bind to delegate, exec when call
	BroadcastDelegate.BindLambda(
		[this, AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
		{
			FAuraAbilityInfo Info =  AbilityInfo->FindAbilityInfoForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec)); //   static so, == UAuraAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec)
			Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec); // set InputTag

			AbilityInfoDelegate.Broadcast(Info); //send to widget
		});

	AuraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);	 // function making call 
	
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
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
