// Copyright  Mateusz Iwanek

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */

class UAuraWidgetController;

UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController; //object if want change controller to diffrent one

protected:

	UFUNCTION(BlueprintImplementableEvent) //definition in BP
	void WidgetControllerSet(); // custom, similar to BeginPlay, call when ready


};
