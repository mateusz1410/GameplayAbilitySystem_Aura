// Copyright  Mateusz Iwanek


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"
#include "GameplayTagContainer.h"
#include "Input/AuraInputComponent.h"
#include "Input/AuraInputConfig.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"

AAuraPlayerController::AAuraPlayerController()
{

	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	

}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

}


void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;

	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit) return;

	ThisActor = CursorHit.GetActor(); // when TScriptInterface < ...> CursorHit.GetActor(), instedof *  Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	* Line trace from cursor. There are several scenarios:
	* 
	* 1. LastActor is null and ThisActor is null
	*		- Do nothing
	* 2. LastActor is null and ThisActor is valid
	*		-Highlight ThisActor
	* 3. LastActor is valid and ThisActor is null
	*		-UnHighlight LastActor
	* 4. Both actor are valid, but LastActor != ThisActor
	*		-UnHighlight LastActor, and Highlight ThisActor
	* 5. Both actors are valid, and are the same actor
	*		-Do nothing
	* 
	*/

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// case 2
			ThisActor->HighlightActor();
		}
		else
		{
			//case 1, do nothing
		}
	}
	else //LastActor is valid
	{
		if (ThisActor == nullptr)
		{
			//case 3
			LastActor->UnHighlightActor();
		}
		else // both are valid
		{
			if (LastActor != ThisActor)
			{
				//case 4

				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				//case 5, do nothing
			}
		}
	}

	LastActor = ThisActor;

}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
			return;

		}
	}

	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else //click to move
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		FHitResult Hit;
		if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
		{
			CachedDestination = Hit.ImpactPoint;
		}
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);

		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
			//Cast<UAuraAbilitySystemComponent>(GetPlayerState<AAuraPlayerState>()->GetAbilitySystemComponent());
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

}




void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfing, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}


