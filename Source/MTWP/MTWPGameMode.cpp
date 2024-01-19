// Copyright Epic Games, Inc. All Rights Reserved.

#include "MTWPGameMode.h"
#include "MTWPCharacter.h"
#include "MTWPAudioSubsystem.h"
#include "MTWPGameInstance.h"
#include "MTWPAudioInterface_WWise.h"
#include "UObject/ConstructorHelpers.h"

AMTWPGameMode::AMTWPGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AMTWPGameMode::BeginPlay()
{
	if (auto GI = GetGameInstance<UMTWPGameInstance>(); IsValid(GI))
	{
		if (auto AS = GI->GetSubsystem<UMTWPAudioSubsystem>(); IsValid(AS) && IsValid(AS->WWiseAudioInterface))
		{
			if (!!!IsValid(GI->AmbientAudioInstance))
			{
				GI->AmbientAudioInstance = AS->WWiseAudioInterface->CreateAudioInstance2D(GI->AmbientAudioEntity, true);
				GI->AmbientAudioInstance->Play();
			}
		}
    }
}
