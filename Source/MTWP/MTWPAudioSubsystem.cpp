#include "MTWPAudioSubsystem.h"


template<typename AudioSystem>
typename AudioSystem::AudioInstanceDefinition UMTWPAudioSubsystem::PlaySound(const typename AudioSystem::AudioEventDefinition& InAudioEventDefinition, const typename AudioSystem::AudioCreationParamsDefinition& InCreationParams, const typename AudioSystem::SwitchDefenition& InSwitchDefenition, const TArray<typename AudioSystem::RtpcDefenition>& RtpcDefenitions)
{
	if (!!!GetWorld())
	{
		return AudioInstanceDefinition();
	}

	auto AS = GetAudioSystem<AudioSystem>();
	if (!!!AS)
	{
		return AudioInstanceDefinition();
	}

	if (!!!AS->IsPlaybackAllowed())
	{
		return AudioInstanceDefinition();
	}

	AudioInstanceDefinition ResultAudioInstance();

	// 2D
	if (InCreationParams.IsPlaying2D)
	{
		if (!IsValid(GetWorld()->GetFirstPlayerController()))
		{
			return nullptr;
		}
		ResultAudioInstance = AS->Play2D(InEventDifinition, InAudioCreationParamsDefinition, InAudioPlaybackParamsDefinition);
	}
	// Attached
	else if (IsValid(InCreationParams.SceneComponent))
	{
		ResultAudioInstance = AS->PlayAttached(InEventDifinition, InAudioCreationParamsDefinition, InAudioPlaybackParamsDefinition);
	}
	// At location
	else
	{
		ResultAudioInstance = AS->PlayAtLocation(InEventDifinition, InAudioCreationParamsDefinition, InAudioPlaybackParamsDefinition);
	}

	if (ResultAudioInstance.IsValid())
	{
		AS->SetPlaybackParams(ResultAudioInstance, InAudioPlaybackParamsDifinition);
	}

	return AudioInstanceDefinition;
}


template<typename AudioSystem>
AudioSystem* UMTWPAudioSubsystem::GetAudioSystem()
{
	// TODO: SCRUM-2: Create newly created system containers and provide getter
	// // TEMP 1:
	if (AudioSystem_WWise.IsValid())
	{
		return AudioSystem_WWise;
	}
	AudioSystem_WWise = MakeUnique<UMTWPAudioSystem_WWise>();
	return AudioSystem_WWise.Get();
	// // END_TEMP 1;
}