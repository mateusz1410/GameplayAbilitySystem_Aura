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
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; //.. in character class

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


};
