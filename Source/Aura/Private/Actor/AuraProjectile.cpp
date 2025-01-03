// Copyright  Mateusz Iwanek


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"



AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	
	Sphere->SetCollisionObjectType(ECC_Projectile);
	
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);

	if(LoopingSound)
	{
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
	}
	
	SetLifeSpan(LifeSpan);
	
}

void AAuraProjectile::Destroyed()
{
	if (!bHit && !HasAuthority()) // actor can be destroyed on client before play effect, so add this
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if (LoopingSoundComponent && LoopingSoundComponent->IsPlaying())
		{
			LoopingSoundComponent->Stop();
		}

	}

	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!DamageEffectSpecHandle.Data.IsValid() || DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
	{
		return; //DamageEffectSpecHandle.Data.IsValid()  not valid on client
	}
	if (!UAuraAbilitySystemLibrary::IsNotFriend(DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(),OtherActor ))
	{
		return;
	}
	if (!bHit)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(),FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect, GetActorLocation());
		if (LoopingSoundComponent && LoopingSoundComponent->IsPlaying())
		{
			LoopingSoundComponent->Stop();
		}
	}
	
		// FString name = UEnum::GetValueAsString( GetLocalRole());
		// GEngine->AddOnScreenDebugMessage(1, 10, FColor::Green, name);

	if (HasAuthority())
	{
		//GE is replicated
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		
		Destroy();
	}
	else
	{
		bHit = true;// client played effect
	}
}

