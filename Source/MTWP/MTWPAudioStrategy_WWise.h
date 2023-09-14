#pragma once

#include "CoreMinimal.h"

#include <AkComponent.h>
#include <AkAudioEvent.h>
#include <AkSwitchValue.h>
#include <AkRtpc.h>

#include "MTWPAudioSubsystem.h"

#include "MTWPAudioStrategy_WWise.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPAudioInstanceDefinition_WWise : public FMTWPAudioInstanceDefinition_Base
{
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAkComponent> Component = nullptr;


	// TODO: SCRUM-XXX: Should be AK_INVALID_PLAYING_ID after Component stops.
	UPROPERTY(EditDefaultsOnly)
	uint32 PlayingID = AK_INVALID_PLAYING_ID;
};


USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPAudioEventDefinition_WWise : public FMTWPAudioEventDefinition_Base
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAkAudioEvent> Event = nullptr;
};


USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPAudioCreationParamsDefinition_WWise : public FMTWPAudioCreationParamsDefinition_Base
{
	GENERATED_BODY()
};


USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPSwitchDefinition_Wwise
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAkSwitchValue> Switch = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FString SwtitchGroup = "";
};


USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPRtpcDefinition_WWise
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAkRtpc> Rtpc = nullptr;

	UPROPERTY(EditAnywhere)
	float Value = 0;
};


USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPAudioPlaybackParamsDefinition_WWise : public FMTWPAudioPlaybackParamsDefinition_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FMTWPSwitchDefinition_Wwise SwitchDefinition;

	UPROPERTY(EditAnywhere)
	FMTWPRtpcDefinition_WWise RtpcDefinition;

	virtual bool IsValid() const override { return true; }
};


UCLASS(Blueprintable, BlueprintType)                 // protected
class MTWP_API UMTWPAudioSystem_WWise : public UObject, public IFMTWPAudioSystem_Base
{
	GENERATED_BODY()

public:
	using AudioInstanceDefinition = FMTWPAudioInstanceDefinition_WWise;
	using AudioEventDefinition = FMTWPAudioEventDefinition_WWise;
	using AudioCreationParamsDefinition = FMTWPAudioCreationParamsDefinition_WWise;
	using AudioPlaybackParamsDefinition = FMTWPAudioPlaybackParamsDefinition_WWise;

	bool IsPlaybackAllowed() { return false; }
	AudioInstanceDefinition Play2D(const AudioEventDefinition& InEventDifinition, const AudioCreationParamsDefinition& InAudioCreationParamsDefinition = AudioCreationParamsDefinition(), const AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition = AudioPlaybackParamsDefinition()) { return AudioInstanceDefinition(); }
	AudioInstanceDefinition PlayAttached(const AudioEventDefinition& InEventDifinition, const AudioCreationParamsDefinition& InAudioCreationParamsDefinition, const AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition = AudioPlaybackParamsDefinition()) { return AudioInstanceDefinition(); }
	AudioInstanceDefinition PlayAtLocation(const AudioEventDefinition& InEventDifinition, const AudioCreationParamsDefinition& InAudioCreationParamsDefinition, const AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition = AudioPlaybackParamsDefinition()) { return AudioInstanceDefinition(); }
	void SetPlaybackParams(AudioInstanceDefinition InAudioInstanceDefinition, const AudioPlaybackParamsDefinition InAudioPlaybackParamsDifinition) { }
};