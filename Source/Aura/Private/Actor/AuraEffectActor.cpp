// Copyright  Mateusz Iwanek


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
	
}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResault)
{
	//TEST ONLY
	// 
	//TODO: Change this to apply a Gameplay Effect. ussing const_cast as hack! is bad
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
		check(AuraAttributeSet); // every actor impplent ASCInterface gots UAuraAttributeSet in this case
		
		
		UAuraAttributeSet* MutableAuraAttributeSet = const_cast<UAuraAttributeSet*>(AuraAttributeSet);// cast to remove const
		MutableAuraAttributeSet->SetHealth(AuraAttributeSet->GetHealth() + 25.f); // const shoudn't be set
		MutableAuraAttributeSet->SetMana(AuraAttributeSet->GetMana() - 25.f); // const shoudn't be set
		Destroy();
	}
}

void AAuraEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::OnEndOverlap);
	
}

