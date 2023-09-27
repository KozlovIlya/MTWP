#include "MTWPAudioSubsystem.h"


#include "MTWPAudioStrategy_WWise.h"

template<typename AudioSystem>
typename AudioSystem::AudioInstanceDefinition UMTWPAudioSubsystem::PlaySound(const typename AudioSystem::AudioEventDefinition& InAudioEventDefinition, const typename AudioSystem::AudioCreationParamsDefinition& InCreationParams, const typename AudioSystem::AudioPlaybackParamsDefinition)
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
		ResultAudioInstance = AS->Create2D(InEventDifinition, InAudioCreationParamsDefinition, InAudioPlaybackParamsDefinition);
	}
	// Attached
	else if (IsValid(InCreationParams.SceneComponent))
	{
		ResultAudioInstance = AS->CreateAttached(InEventDifinition, InAudioCreationParamsDefinition, InAudioPlaybackParamsDefinition);
	}
	// At location
	else
	{
		ResultAudioInstance = AS->CreateAtLocation(InEventDifinition, InAudioCreationParamsDefinition, InAudioPlaybackParamsDefinition);
	}

	if (ResultAudioInstance.IsValid())
	{
		AS->SetPlaybackParams(ResultAudioInstance, InAudioPlaybackParamsDifinition);
		AudioSystem.Play(ResultAudioInstance, InAudioEventDefinition);
	}

	return AudioInstanceDefinition;
}

template <typename AudioSystem>
AudioSystem* UMTWPAudioSubsystem::GetAudioSystem()
{
	if (auto IdxPtr = ActiveAudioSystemMap.Find(AudioSystem::StaticClass()); IdxPtr)
	{
		return Cast<AudioSystem>(ActiveAudioSystems[*IdxPtr].Get());
	}

	ActiveAudioSystemMap.Enque(AudioSystem::StaticClass()::GetName(), TActiveAudioSystems.Add(MakeUnique<AudioSystem>()));
	return Cast<AudioSystem>(ActiveAudioSystemMap[TActiveAudioSystems.GetLength() - 1].Get());
}
