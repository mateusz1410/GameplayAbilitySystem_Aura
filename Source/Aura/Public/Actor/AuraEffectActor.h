// Copyright  Mateusz Iwanek

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
struct FActiveGameplayEffectHandle;
class UAbilitySystemComponent;

UENUM(BlueprintType) // how effect will be apply
enum class EEffectApplicationPolicy : uint8
{
	EEAP_OnOverlap  UMETA(DisplayName = "ApplyOnOverlap"),
	EEAP_OnEndOverlap  UMETA(DisplayName = "ApplyOnEndOverlap"),
	EEAP_DoNotApply  UMETA(DisplayName = "DoNotApply")
};

UENUM(BlueprintType) // how effect will be remove
enum class EEffectRemovalPolicy : uint8
{
	EERP_RemoveOnEndOverlap  UMETA(DisplayName = "RemoveOnEndOverlap"),
	EERP_DoNotRemove  UMETA(DisplayName = "DoNotRemove")
};



UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void ApplyMultipleEffectsToTarget(AActor* TargetActor, TArray<TSubclassOf<UGameplayEffect>> GameplayEffectClassArray);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectRemoval = false;

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

#pragma region InstantGameplayEffect

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (ToolTip = "List of instant GE apply on target"))
	TArray<TSubclassOf<UGameplayEffect>> InstantGameplayEffectClass; // applied immediately  // before TSubclassOf<UGameplayEffect>  TArray<...> for testing 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (ToolTip = "Instant GE apply"))
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::EEAP_DoNotApply;

#pragma endregion

	//PeriodGameplayEffect DurationGE and InfiniteGE + Period

#pragma region DurationGameplayEffect


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (ToolTip = "List of duration GE apply on target"))
	TArray<TSubclassOf<UGameplayEffect>> DurationGameplayEffectClass;//applied immediately and exists for duration then remove (no on target)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (ToolTip = "duration GE apply"))
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::EEAP_DoNotApply;;

	
#pragma endregion

#pragma region InfiniteGameplayEffect

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects" , meta = (ToolTip = "List of infinite GE apply on target"))
	TArray <TSubclassOf<UGameplayEffect>> InfiniteGameplayEffectClass;//applied immediately and exists forever until it's removed

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (ToolTip = "infinite GE apply"))
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::EEAP_DoNotApply;;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (ToolTip = "infinite GE remove"))
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::EERP_RemoveOnEndOverlap;

#pragma endregion


	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (ToolTip = "Value set in GE when affect is apply. Can be used in CurveTabel, ActorLevel* SlalableFloatMaginitude "))
	float ActorLevel=1.f; //GE*ActorLevel CurveTable

};
