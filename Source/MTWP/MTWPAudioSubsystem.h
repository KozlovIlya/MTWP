#pragma once

#include <CoreMinimal.h>
#include <Subsystems/GameInstanceSubsystem.h>

#include "MTWPAudioSubsystem.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPAudioCreationParamsDefinition_Base
{
	GENERATED_BODY()

		// 2D, will be attached to player controller
		inline FMTWPAudioCreationParamsDefinition_Base(const bool InCrossLevel = false) :
		Playing2D(true),
		CrossLevel(InCrossLevel)
	{}

	// 3D Play on location
	inline FMTWPAudioCreationParamsDefinition_Base(const FVector InLocation) :
		Location(InLocation)
	{}

	// 3D attach to component
	inline FMTWPAudioCreationParamsDefinition_Base(USceneComponent* InSceneComponent,
		const FName InAttachPointName = NAME_None,
		const FRotator InRotation = FRotator(ForceInit),
		const FVector InLocation = FVector(ForceInit),
		const EAttachLocation::Type InAttachLocation = EAttachLocation::KeepRelativeOffset) :
		SceneComponent(InSceneComponent),
		AttachPointName(InAttachPointName),
		Location(InLocation),
		Rotation(InRotation),
		AttachLocation(InAttachLocation)
	{}

	//void Play();

	virtual bool IsValid() const { return true; }

protected:

	UPROPERTY()
		UObject* ContextObject;
	UPROPERTY()
		TObjectPtr<USceneComponent> SceneComponent = nullptr;
	UPROPERTY()
		FName AttachPointName = NAME_None;
	UPROPERTY()
		FRotator Rotation = FRotator(ForceInit);
	UPROPERTY()
		FVector Location = FVector(ForceInit);
	UPROPERTY()
		TEnumAsByte<EAttachLocation::Type> AttachLocation;

	bool Playing2D = false;
	bool CrossLevel = false;


	friend class UMTWPAudioSubsystem;
};

USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPAudioInstanceDefinition_Base
{
	GENERATED_BODY()

	virtual bool IsValid() const { return true; }
};

USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPAudioEventDefinition_Base
{
	GENERATED_BODY()

		virtual bool IsValid() const { return true; }
};

USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPAudioPlaybackParamsDefinition_Base
{
	GENERATED_BODY()

		virtual bool IsValid() const { return true; }
};

UINTERFACE(BlueprintType, Blueprintable)
class MTWP_API UFMTWPAudioSystem_Base : public UInterface
{
	GENERATED_BODY()
};

//UCLASS(BlueprintType, Blueprintable)
class MTWP_API IFMTWPAudioSystem_Base
{
	GENERATED_BODY()

		// Must be declared same way
	using AudioInstanceDefinition = FMTWPAudioInstanceDefinition_Base;
	using AudioEventDefinition = FMTWPAudioEventDefinition_Base;
	using AudioCreationParamsDefinition = FMTWPAudioCreationParamsDefinition_Base;
	using AudioPlaybackParamsDefinition = FMTWPAudioPlaybackParamsDefinition_Base;

	// Just copy-paste it into your implementation
	bool IsPlaybackAllowed() { return false; }
	AudioInstanceDefinition Create2D(const AudioEventDefinition& InEventDifinition, const AudioCreationParamsDefinition& InAudioCreationParamsDefinition = AudioCreationParamsDefinition(), const AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition = AudioPlaybackParamsDefinition()) { return AudioInstanceDefinition(); }
	AudioInstanceDefinition CreateAttached(const AudioEventDefinition& InEventDifinition, const AudioCreationParamsDefinition& InAudioCreationParamsDefinition, const AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition = AudioPlaybackParamsDefinition()) { return AudioInstanceDefinition(); }
	AudioInstanceDefinition CreateAtLocation(const AudioEventDefinition& InEventDifinition, const AudioCreationParamsDefinition& InAudioCreationParamsDefinition, const AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition = AudioPlaybackParamsDefinition()) { return AudioInstanceDefinition(); }
	void SetPlaybackParams(AudioInstanceDefinition InAudioInstanceDefinition, const AudioPlaybackParamsDefinition InAudioPlaybackParamsDifinition) { }
	void Play(AudioInstanceDefinition& InAudioInstanceDefinition, const AudioEventDefinition& InEventDifinition) { }
};

UCLASS(Blueprintable, BlueprintType) // Abstract?
class MTWP_API UMTWPAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	template <typename AudioSystem>
	typename AudioSystem::AudioInstanceDefinition PlaySound(const typename AudioSystem::AudioEventDefinition& InAudioEventDefinition, const typename AudioSystem::AudioCreationParamsDefinition& InCreationParams = AudioSystem::AudioCreationParamsDefinition(), const typename AudioSystem::AudioPlaybackParamsDefinition = AudioSystem::AudioPlaybackParamsDefinition());

protected:

	template <typename AudioSystem>
	AudioSystem* GetAudioSystem();

	//// Classname + ActiveAudioSystems index
	//TMap<TSubclassOf<UFMTWPAudioSystem_Base>, uint8> ActiveAudioSystemMap;

	//// UnReAl EnGiNe MaGiC: Garbage collector watches UObject* and array of UObject* only
	//TArray<TUniquePtr<UFMTWPAudioSystem_Base>> ActiveAudioSystems;
};

//template <typename AudioSystem>
//typename AudioSystem::AudioInstanceDefinition UMTWPAudioSubsystem::PlaySound(const typename AudioSystem::AudioEventDefinition& InAudioEventDefinition, const typename AudioSystem::AudioCreationParamsDefinition& InCreationParams, const typename AudioSystem::AudioPlaybackParamsDefinition InPlaybackParams)
//{
//	if (!InAudioEventDefinition.IsValid())
//	{
//		UE_LOG(LogMTWP, Error, TEXT("Invalid AudioEventDefinition"));
//		return AudioSystem::AudioInstanceDefinition();
//	}
//
//	if (!InCreationParams.IsValid())
//	{
//		UE_LOG(LogMTWP, Error, TEXT("Invalid AudioCreationParamsDefinition"));
//		return AudioSystem::AudioInstanceDefinition();
//	}
//
//	if (!InPlaybackParams.IsValid())
//	{
//		UE_LOG(LogMTWP, Error, TEXT("Invalid AudioPlaybackParamsDefinition"));
//		return AudioSystem::AudioInstanceDefinition();
//	}
//
//	AudioSystem* AudioSystemPtr = GetAudioSystem<AudioSystem>();
//	if (!AudioSystemPtr)
//	{
//		UE_LOG(LogMTWP, Error, TEXT("AudioSystem not found"));
//		return AudioSystem::AudioInstanceDefinition();
//	}
//
//	if (!AudioSystemPtr->IsPlaybackAllowed())
//	{
//		UE_LOG(LogMTWP, Error, TEXT("Playback not allowed"));
//		return AudioSystem::AudioInstanceDefinition();
//	}
//
//	AudioSystem::AudioInstanceDefinition AudioInstanceDefinition;
//	if (InCreationParams.Playing2D)
//	{
//		AudioInstanceDefinition = AudioSystemPtr->Create2D(InAudioEventDefinition, InCreationParams, InPlaybackParams);
//	}
//	else if (InCreationParams.SceneComponent)
//	{
//		AudioInstanceDefinition = AudioSystemPtr->CreateAttached(InAudioEventDefinition, InCreationParams, InPlaybackParams);
//	}
//	else
//	{
//		AudioInstanceDefinition = AudioSystemPtr->CreateAtLocation(InAudioEventDefinition, InCreationParams, InPlaybackParams);
//	}
//
//	if (!AudioInstanceDefinition.IsValid())
//	{
//		UE_LOG(LogMTWP, Error, TEXT("AudioInstanceDefinition is invalid"));
//		return AudioSystem::AudioInstanceDefinition();
//	}
//
//	AudioSystemPtr->Play(AudioInstanceDefinition, InAudioEventDefinition);
//
//	return AudioInstanceDefinition;
//}
//
//template <typename AudioSystem>
//AudioSystem* UMTWPAudioSubsystem::GetAudioSystem()
//{
//	//uint8 AudioSystemIndex = ActiveAudioSystemMap.FindRef(AudioSystem::StaticClass());
//	//if (ActiveAudioSystems.IsValidIndex(AudioSystemIndex))
//	//{
//	//	return Cast<AudioSystem>(ActiveAudioSystems[AudioSystemIndex].Get());
//	//}
//
//	return nullptr;
//}< / code>
//
//
//
//I'm trying to make a generic audio system for my game.
//I want to have a base class for audio system, and then have a bunch of derived classes for different audio systems.
//I want to have a base class for audio creation params, and then have a bunch of derived classes for different audio systems.
//I want to have a base class for audio instance, and then have a bunch of derived classes for different audio systems.
//I want to have a base class for audio event, and then have a bunch of derived classes for different audio systems.
//I want to have a base class for audio playback params, and then have a bunch of derived classes for different audio systems.
//I want to have a base class for audio subsystem, and then have a bunch of derived classes for different audio systems.
//
//I'm not sure how to go about this. Any help would be appreciated. Thanks!