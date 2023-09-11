#pragma once

#include <CoreMinimal.h>
#include <Subsystems/GameInstanceSubsystem.h>

#include "MTWPAudioSubsystem.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPAudioCreationParamsDefinition_Base
{
	GENERATED_BODY()

	// 2D, will be attached to player controller
	inline FMTWPAudioCreationParams_Base(const bool InCrossLevel = false) :
		Playing2D(true),
		CrossLevel(InCrossLevel)
	{}

	// 3D Play on location
	inline FMTWPAudioCreationParams_Base(const FVector InLocation) :
		Location(InLocation)
	{}

	// 3D attach to component
	inline FMTWPAudioCreationParams_Base(USceneComponent* InSceneComponent,
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
struct MTWP_API FMTWPAudioComponentDefinition_Base
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
class MTWP_API IFMTWPAudioSystem_Base : public UInterface
{
	GENERATED_BODY()
};

//UCLASS(BlueprintType, Blueprintable)
class MTWP_API IFMTWPAudioSystem_Base
{
	GENERATED_BODY()

public:

	// Must be declared same way
	using AudioComponentDefinition = FMTWPAudioComponentDefinition_Base;
	using AudioEventDefinition = FMTWPAudioEventDefinition_Base;
	using AudioCreationParamsDefinition = FMTWPAudioCreationParamsDefinition_Base;
	using AudioPlaybackParamsDefinition = FMTWPAudioPlaybackParamsDefinition_Base;


	// Must be declared and implemented in inheritors the same way
	bool IsPlaybackAllowed(UWorld* World) { return false; }
	AudioComponentDefinition Play2D(AudioEventDefinition& InEventDifinition, AudioCreationParamsDefinition& InAudioCreationParamsDefinition = AudioCreationParamsDefinition(), AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition = AudioPlaybackParamsDefinition()) { return AudioComponentDefinition(); }
	AudioComponentDefinition PlayAttached(AudioEventDefinition& InEventDifinition, AudioCreationParamsDefinition& InAudioCreationParamsDefinition, AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition = AudioPlaybackParamsDefinition()) { return AudioComponentDefinition(); }
	AudioComponentDefinition PlayAtLocation(AudioEventDefinition& InEventDifinition, AudioCreationParamsDefinition& InAudioCreationParamsDefinition, AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition = AudioPlaybackParamsDefinition()) { return AudioComponentDefinition(); }
	void SetPlaybackParams(AudioComponentDefinition InAudioComponentDefinition, AudioPlaybackParamsDefinition InAudioPlaybackParamsDifinition) { }
};






//USTRUCT(BlueprintType, Blueprintable)
//struct MTWP_API FMTWPRtpcDefenition
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditDefaultsOnly)
//	TObjectPtr<class UAkRtpc> Rtpc;
//
//	UPROPERTY(EditDefaultsOnly)
//	float Value;
//
//	UPROPERTY(EditDefaultsOnly)
//	float InterpolationSeconds;
//
//	UPROPERTY(EditDefaultsOnly)
//	float MaxGameValue;
//
//	UPROPERTY(BlueprintReadOnly)
//	float DeclaredMaxSoundEngnValue = 100;
//};
//
//
//USTRUCT()
//struct MTWP_API FMTWPAudioPlaybackParams
//{
//	GENERATED_BODY()
//
//	UPROPERTY()
//	float CooldownSeconds = 0;
//
//	UPROPERTY()
//	TObjectPtr<class UAkSwitchValue> SwitchValue = nullptr;
//
//	UPROPERTY()
//	FString SwitchGroupName = "";
//
//	UPROPERTY()
//	TArray<FMTWPRtpcDefenition> RtpcDefinitions;
//};


UCLASS(Blueprintable, BlueprintType) // Abstract?
class MTWP_API UMTWPAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	// TODO: SCRUM-1: Separate concrete audio systems declaration
	// TODO: SCRUM-2: Create newly created system containers and provide getter
	UPROPERTY(BlueprintReadOnly)
	TUniquePtr<class UMTWPAudioSystem_WWise> AudioSystem_WWise;

	template <typename AudioSystem>
	typename AudioSystem::AudioComponentDefinition* PlaySound(const typename AudioSystem::AudioEventDefinition& InAudioEventDefinition, const typename AudioSystem::AudioCreationParamsDefinition& InCreationParams = FMTWPAudioCreationParams(), const typename AudioSystem::SwitchDefenition& InSwitchDefenition = AudioSystem::SwitchDefenition(), const TArray<typename AudioSystem::RtpcDefenition>& RtpcDefenitions = TArray< AudioSystem::RtpcDefenition>());

	// TODO: SCRUM-2: Create newly created system containers and provide getter
	//UPROPERTY(BlueprintReadonly)
	//TMap<FName, TObjectPtr<IFMTWPAudioSystem_Base>> AudioSystemMap;

protected:

	template <typename AudioSystem>
	AudioSystem* GetAudioSystem();

	// TODO: SCRUM-2: Create newly created system containers and provide getter
	//UPROPERTY(BlueprintReadonly)
	//TArray<TObjectPtr<IFMTWPAudioSystem_Base>> ActiveAudioSystems;
};
