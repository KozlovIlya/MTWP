#pragma once

#include "CoreMinimal.h"

#include "MTWPAudioSubsystem.h"
#include "MTWPAudioInterface_WWise.generated.h"


UCLASS (BlueprintType, Blueprintable)
class MTWP_API UMTWPAudioInstance_WWise : public UMTWPAudioInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | WWise")
	virtual bool IsValid() const override;

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | WWise")
	virtual void Play() override;

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | WWise")
	virtual void Stop() override;

	//UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	//virtual void Pause() = 0;

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | WWise")
	virtual void SetPlaybackParameters(const UMTWPPlaybackParameters* InPlaybackParameters);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AudioInstance | WWise")
	TObjectPtr<class UAkComponent> Component = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "AudioInstance | WWise")
	int PlayingID = AK_INVALID_PLAYING_ID;

	UPROPERTY(BlueprintReadOnly, Category = "AudioInstance | WWise")
	int ActiveEventID = AK_INVALID_UNIQUE_ID; // UAkEvent in ActivePlaybackParameters may be destroyed so we contain it's id.

	friend UMTWPAudioInterface_WWise;
};

USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API UMTWPSwitch_Wwise
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAkSwitchValue> Switch = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FString SwitchGroup = "";

	bool IsValid() const;

protected:
	mutable FString NameString = "";

	friend FMTWPAudioInstance_WWise;
};

USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API UMTWPRtpc_WWise
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UAkRtpc> Rtpc = nullptr;

	UPROPERTY(EditAnywhere)
	float Value = 0;

	UPROPERTY(EditDefaultsOnly)
	uint32 InterpolationTimeMs = 0;

	bool IsValid() const;

protected:
	mutable uint32 Id = 0;
	mutable FString NameString = "";

	friend FMTWPAudioInstance_WWise;
};

USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API UMTWPPlaybackParameters_WWise : public UMTWPPlaybackParameters
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAkAudioEvent> Event;

	UPROPERTY(EditDefaultsOnly)
	UMTWPSwitch_Wwise Switch;

	UPROPERTY(EditDefaultsOnly)
	TSet<FMTWPRtpc_WWise> RtpcSet;

	UPROPERTY(EditDefaultsOnly, Category = "PlaybackParameters | AcionParameters")
	EAkCurveInterpolation FadeInCurveType = EAkCurveInterpolation::Linear;

	UPROPERTY(EditDefaultsOnly, Category = "PlaybackParameters | AcionParameters")
	EAkCurveInterpolation FadeOutCurveType = EAkCurveInterpolation::Linear;
};

UCLASS(Blueprintable, BlueprintType)
class MTWP_API UMTWPAudioInterface_WWise : public UMTWPAudioInterface
{
	GENERATED_BODY()

public:

	virtual UMTWPAudioInstance* CreateAudioInstance2D(const UMTWPPlaybackParameters* PlaybackParameters, const bool bCrossWorld = false) override;
	
	virtual UMTWPAudioInstance* CreateAudioInstanceAtLocation(const UMTWPPlaybackParameters* AudioEntity, const FVector& Location, const FRotator& Orientation = FRotator(ForceInit)) override;
	
	virtual UMTWPAudioInstance* CreateAudioInstanceAttached(const UMTWPPlaybackParameters* AudioEntity, USceneComponent* AttachComponent, FName AttachPointName = NAME_None, const FVector& Location = FVector(ForceInit), const FRotator& Orientation = FRotator(ForceInit), EAttachmentRule AttachmentRule = EAttachmentRule::KeepRelative) override;
};
