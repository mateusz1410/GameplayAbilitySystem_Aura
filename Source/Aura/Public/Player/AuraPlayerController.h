// Copyright  Mateusz Iwanek

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/EnemyInterface.h"
#include "AuraPlayerController.generated.h"

/**
 * 
 */

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraInputConfig;
struct FGameplayTag;
class UAuraAbilitySystemComponent;
class USplineComponent;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime);

protected:
	virtual void BeginPlay() override;


	virtual void SetupInputComponent() override;
	//insted of 
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; //.. in 
	// haracter class

private:
#pragma region InputAction/Mapping

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);

#pragma endregion

	void CursorTrace();

	TScriptInterface<IEnemyInterface> LastActor; // insted of IEnemyInterface*
	TScriptInterface <IEnemyInterface> ThisActor; // insted of IEnemyInterface*
	FHitResult CursorHit;

//------------------------------------- Ability ---------------------

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfing;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

//----------------- Click To Move ------

	FVector CachedDestination = FVector::Zero(); // click location 3D

	float FollowTime = 0.f; //how long button is pressed
	float ShortPressThreshold = 0.5f; //click to move or hold to follow cusor, threshold
	bool bAutoRunning = false; // move to location under cursor, not follow cursor
	bool bTargeting = false; // mouse over someActor
	
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun(); // move to clicked location

};
