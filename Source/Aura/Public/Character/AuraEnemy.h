// Copyright  Mateusz Iwanek

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

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

#pragma region IEnemyInterface

	// Inherited via IEnemyInterface
	void HighlightActor() override;
	void UnHighlightActor() override;

#pragma endregion

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

protected:
	virtual void InitAbilityActorInfo() override;

};
