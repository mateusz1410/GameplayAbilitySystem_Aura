// Copyright  Mateusz Iwanek


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data) 
			{

				BroadcastAttributeInfo(Pair.Key, Pair.Value());

			}); // Pair by value, value when function was call
	}

}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);

}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet); //get AS from parent
	
	check(AttributeInfo);
	
	/* old version
	
		check(AttributeInfo);
		FAuraAttributeInfo StrengthInfo = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength); // get info from DA about Strength
		StrengthInfo.AttributeValue = AS->GetStrength();// change Info AttributeValue for value in AS, now Info got all information

		AttributeInfoDelegate.Broadcast(StrengthInfo); // broadcast, Widget bind

	*/

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());//  Pair.Value is Function pointer, exec function with ()

		/* 
			//FunctionPointer = GetIntelligenceAttribute; // set function pointer to function GetIntelligenceAttribute
			//FGameplayAttribute Attr = FunctionPointer(); //execute
		*/

	}
}
