// Copyright  Mateusz Iwanek


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"


//UAuraAssetManager UAuraAssetManager::AuraAssetManager; there is UAssetManager UE class

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	//set UAuraAssetManager in  file Config->DefaultEngine.ini  in section [/Script/Engine.Engine] add line AssetManagerClassName = / Script / Aura.AuraAssetManager

	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();// add Native GameplayTags
}
