
// Copyright  Mateusz Iwanek

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class AAuraAIController;
class UWidgetComponent;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

public:

	AAuraEnemy();
	virtual void PossessedBy(AController* NewController) override;

#pragma region IEnemyInterface

	/** Inherited via IEnemyInterface */
	void HighlightActor() override;
	void UnHighlightActor() override;
	/**  end Enemy Interface  */

	/** Combat Interface*/
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die() override; 
	/** end Comabt Interface*/



#pragma endregion

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged; // DelegateSignature from OverlayWidget

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged; // DelegateSignature from OverlayWidget
	
	
	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

	void HitReactTagChanged( const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	//---------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f; //after Die

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	
protected:
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character class Defaults")
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character class Defaults")
	TObjectPtr<UWidgetComponent> HealthBar;

	/***** AI ****************/
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
};
