// Copyright  Mateusz Iwanek


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObject = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return MyObject;
}

void UTargetDataUnderMouse::Activate()
{
	//Super::Activate(); do nothing in parent, only UE_VLOG
	//if (Cast<APlayerController>(Cast<APawn>(GetTaskOwner()->GetGameplayTaskAvatar(this))->GetController())->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, res))
	
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

	if (bIsLocallyControlled)
	{
		SendMouseCursorData();// send TargetData,only client know his mouse position
	}
	else
	{
		/******** We are on the server, so listen for target data.  ************/
		//two cases, one ability is Activated before call with TargetData, or call with TargetData is before Activation

		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();

		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).
			AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback); //bind for  get TargetDataand exec

		//call with TargetData was before bind OnTargetDataReplicatedCallback. if so CallReplicatedData
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);// return bool if server have TargetData

		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData(); // wait for call TargetData
		}
	}

}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get()); // create synchronization point for Client and server, time/value

	/**** ****/

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CusrosHit;
	PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CusrosHit);

	/**** ****/

	//--------------------
	FGameplayAbilityTargetDataHandle DataHandle; // add in ...AssetManager::StartInitialLoading() --->  UAbilitySystemGlobals::Get().InitGlobalData()

	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CusrosHit;
	DataHandle.Add(Data);
	//----------------------

	/** Send Data to Server*/
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),// key when activate
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);// use ScopedPrediction

	if (ShouldBroadcastAbilityTaskDelegates()) // check if ability is still active
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey()); //got Data no need to wait and store in cached, SetWaitingOnRemotePlayerData();

	if (ShouldBroadcastAbilityTaskDelegates()) // check if ability is still active
	{
		ValidData.Broadcast(DataHandle);
	}
}
