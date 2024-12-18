// Copyright  Mateusz Iwanek


#include "Character/AuraEnemy.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AuraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"


AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar= CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	
	//Init ability info for the Server and Clients
	check(AbilitySystemComponent);
	InitAbilityActorInfo();
	
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this,AbilitySystemComponent);
	}

	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this); // next in widget -> WidgetControllerSet Delegate-> GetWidgetController->BindOnHealthChanged
	}
	
	/** Bind do atribute change and call delegate with what was changed*/
	if (const UAuraAttributeSet* AuraAS= CastChecked<UAuraAttributeSet>(GetAttributeSet()))
	{
		//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()) // return delegate
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue); // call own delegate, to broadcast it to widget
			}
			);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue); // call own delegate, to broadcast it to widget
			}
			);

		//Listen for tag added/removed
		/** HitReact tag */
		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AAuraEnemy::HitReactTagChanged); // AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effect_HitReact, EGameplayTagEventType::NewOrRemoved) // return delegate
		
		/* Broadcast info about Init value to widget*/
		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
		
	}
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this,CharacterClass,Level,AbilitySystemComponent);
}

void AAuraEnemy::HighlightActor()
{
	bHighlighted = true; //can remove this variable
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED); // CUSTOM_DEPTH_RED from Aura.h

	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	bHighlighted = false;

	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(0);

	Weapon->SetRenderCustomDepth(false);
	Weapon->SetCustomDepthStencilValue(0);
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}
