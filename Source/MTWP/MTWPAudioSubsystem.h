#pragma once

#include <CoreMinimal.h>
#include <Subsystems/GameInstanceSubsystem.h>

#include "MTWPAudioSubsystem.generated.h"

class UAkComponent;
class UAkAudioEvent;


USTRUCT()
struct MTWP_API FMTWPAudioCreationParams
{
	GENERATED_BODY()

	// 2D, will be attached to player controller
	inline FMTWPAudioCreationParams() : IsPlaying2D(true) 
	{}

	// 3D Play on location
	inline FMTWPAudioCreationParams(const FVector InLocation) :
		Location(InLocation)
	{}

	// 3D attach to component
	inline FMTWPAudioCreationParams(USceneComponent* InSceneComponent,
	const FName InAttachPointName = NAME_None,
	const FRotator InRotation = FRotator(ForceInit),
	const FVector InLocation = FVector(ForceInit)) :
		SceneComponent(InSceneComponent),
		AttachPointName(InAttachPointName),
		Location(InLocation),
		Rotation(InRotation)
	{}

protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> SceneComponent = nullptr;
	UPROPERTY()
	FName AttachPointName = NAME_None;
	UPROPERTY()
	FRotator Rotation = FRotator(ForceInit);
	UPROPERTY()
	FVector Location = FVector(ForceInit);

	bool IsPlaying2D = false;

	friend class UMTWPAudioSubsystem;
};


USTRUCT()
struct MTWP_API FMTWPAudioPlaybackParams
{
	GENERATED_BODY()

	UPROPERTY()
	float CooldownSeconds = 0;

	UPROPERTY()
	TObjectPtr<class UAkSwitchValue> SwitchValue = nullptr;
	
	UPROPERTY()
	FString SwitchGroupName = "";
	
};


UCLASS(Blueprintable, BlueprintType)
class MTWP_API UMTWPAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION()
	class UAkComponent* PlaySound(UAkAudioEvent* InAudioEvent, const FMTWPAudioCreationParams InCreationParams = FMTWPAudioCreationParams(), FMTWPAudioPlaybackParams InPlaybackParams = FMTWPAudioPlaybackParams());

protected:

	// For throttling needs
	TMap<uint32_t, float> EventTimingsMap;
};
