// Copyright  Mateusz Iwanek


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer = HasAuthority(&ActivationInfo); //in ability
	if (!bIsServer) return;

	ICombatInterface* ComabtInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (ComabtInterface)
	{
		const FVector SocketLocation = ComabtInterface->GetCombatSocketLocation();
			
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);

		//TODO: Set Projectile rotation

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


		// TODO: Give the Projectile a GESpec for cousing damage

		Projectile->FinishSpawning(SpawnTransform); // GetWorld()->SpawnActorDeferred, wait with spawn until FinishSpawning() is call;
	}

	//UKismetSystemLibrary::PrintString(this,FString("ActivateAbility (C++)"), true, true,FLinearColor::Yellow, 3.f); // diffrent verion of print string, used in BP
}
