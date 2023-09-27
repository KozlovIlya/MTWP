#pragma once

#include "CoreMinimal.h"

#include "MTWPAudioSubsystem.h"

#include "MTWPAudioStrategy_WWise.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPAudioInstanceDefinition_WWise : public FMTWPAudioInstanceDefinition_Base
{
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UAkComponent> Component = nullptr;

	UPROPERTY(BlueprintReadOnly)
	int PlayingID = 0U;

	UPROPERTY(BlueprintReadOnly)
	int EventID = 0U;

	virtual bool IsValid() const override;

	//virtual void Play();

	friend UMTWPAudioSystem_WWise;
};


USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPAudioEventDefinition_WWise : public FMTWPAudioEventDefinition_Base
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UAkAudioEvent> Event;

	virtual bool IsValid() const override;
};


USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPAudioCreationParamsDefinition_WWise : public FMTWPAudioCreationParamsDefinition_Base
{
	GENERATED_BODY()

	friend UMTWPAudioSystem_WWise;
};


USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPSwitchDefinition_Wwise
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAkSwitchValue> Switch = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FString SwitchGroup = "";

	bool IsValid() const;

	friend UMTWPAudioSystem_WWise;
};


USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPRtpcDefinition_WWise
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UAkRtpc> Rtpc = nullptr;

	UPROPERTY(EditAnywhere)
	float Value = 0;

	UPROPERTY(EditDefaultsOnly)
	uint32 InterpolationTimeMs = 0;

	bool IsValid() const;

	friend UMTWPAudioSystem_WWise;
};


USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPAudioPlaybackParamsDefinition_WWise : public FMTWPAudioPlaybackParamsDefinition_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FMTWPSwitchDefinition_Wwise SwitchDefinition;

	UPROPERTY(EditAnywhere)
	FMTWPRtpcDefinition_WWise RtpcDefinition;

	virtual bool IsValid() const override;

	friend UMTWPAudioSystem_WWise;
};

													 // TODO:
UCLASS(Blueprintable, BlueprintType)                 // protected
class MTWP_API UMTWPAudioSystem_WWise : public UObject, public IFMTWPAudioSystem_Base
{
	GENERATED_BODY()

public:
	using AudioInstanceDefinition = FMTWPAudioInstanceDefinition_WWise;
	using AudioEventDefinition = FMTWPAudioEventDefinition_WWise;
	using AudioCreationParamsDefinition = FMTWPAudioCreationParamsDefinition_WWise;
	using AudioPlaybackParamsDefinition = FMTWPAudioPlaybackParamsDefinition_WWise;

	bool IsPlaybackAllowed() const;
	void SetPlaybackParams(AudioInstanceDefinition InAudioInstanceDefinition, const AudioPlaybackParamsDefinition InAudioPlaybackParamsDifinition);

protected:
	AudioInstanceDefinition CreateAudioInstance2D(const AudioEventDefinition& InEventDifinition, const AudioCreationParamsDefinition& InAudioCreationParamsDefinition = AudioCreationParamsDefinition(), const AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition = AudioPlaybackParamsDefinition());
	AudioInstanceDefinition CreateAudioInstanceAttached(const AudioEventDefinition& InEventDifinition, const AudioCreationParamsDefinition& InAudioCreationParamsDefinition, const AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition = AudioPlaybackParamsDefinition());
	AudioInstanceDefinition CreateAudioInstanceAtLocation(const AudioEventDefinition& InEventDifinition, const AudioCreationParamsDefinition& InAudioCreationParamsDefinition, const AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition = AudioPlaybackParamsDefinition());
};
