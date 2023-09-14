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
		const FVector InLocation = FVector(ForceInit)) :
		SceneComponent(InSceneComponent),
		AttachPointName(InAttachPointName),
		Location(InLocation),
		Rotation(InRotation)
	{}

	virtual bool IsValid() const { return true; }

protected:

	UPROPERTY()
	TObjectPtr<USceneComponent> SceneComponent = nullptr;
	UPROPERTY()
	FName AttachPointName = NAME_None;
	UPROPERTY()
	FRotator Rotation = FRotator(ForceInit);
	UPROPERTY()
	FVector Location = FVector(ForceInit);

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

UINTERFACE(BlueprintType , Blueprintable)
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
public:
	bool IsPlaybackAllowed() { return false; }
	AudioInstanceDefinition Play2D(const AudioEventDefinition& InEventDifinition, const AudioCreationParamsDefinition& InAudioCreationParamsDefinition = AudioCreationParamsDefinition(), const AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition = AudioPlaybackParamsDefinition()) { return AudioInstanceDefinition(); }
	AudioInstanceDefinition PlayAttached(const AudioEventDefinition& InEventDifinition, const AudioCreationParamsDefinition& InAudioCreationParamsDefinition, const AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition = AudioPlaybackParamsDefinition()) { return AudioInstanceDefinition(); }
	AudioInstanceDefinition PlayAtLocation(const AudioEventDefinition& InEventDifinition, const AudioCreationParamsDefinition& InAudioCreationParamsDefinition, const AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition = AudioPlaybackParamsDefinition()) { return AudioInstanceDefinition(); }
	void SetPlaybackParams(AudioInstanceDefinition InAudioInstanceDefinition, const AudioPlaybackParamsDefinition InAudioPlaybackParamsDifinition) { }
	// TODO: Do we need Stop(...) method?

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

	// Classname + ActiveAudioSystems index
	TMap<TSubclassOf<UFMTWPAudioSystem_Base>, uint8> ActiveAudioSystemMap;

	// UnReAl EnGiNe MaGiC: Garbage collector watches UObject* and array of UObject* only
	TArray<TUniquePtr<UFMTWPAudioSystem_Base>> ActiveAudioSystems;
};
