#pragma once

#include <CoreMinimal.h>
#include <Subsystems/GameInstanceSubsystem.h>

#include "MTWPAudioSubsystem.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPSwitchDefinition_Base
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPRtpcDefenition_Base
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType, Blueprintable)
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

USTRUCT(BlueprintType, Blueprintable)
struct MTWP_API FMTWPAudioPlaybackParams
{
	GENERATED_BODY()

	UPROPERTY()
	float CooldownSeconds = 0;
};

UINTERFACE(BlueprintType , Blueprintable)
class MTWP_API UFMTWPAudioStrategy : public UInterface
{
	GENERATED_BODY()
};

class MTWP_API IFMTWPAudioStrategy
{
	GENERATED_BODY()

	using AudioComponent = USceneComponent;
	using AudioEvent = UObject;
	using SwitchDefenition = FMTWPSwitchDefinition_Base;
	using RtpcDefenition = FMTWPRtpcDefenition_Base;


	virtual USceneComponent* Play2D(UObject* InEvent) {}
	virtual USceneComponent* PlayAttached(UObject* InEvent, USceneComponent* InAttachComponent, const FName& InAttachSocketName, const FTransform& Transform) {}
	virtual USceneComponent* PlayAtLocation(UObject* InEvent, const FTransform& InTransform) {}
	virtual void SetSwitch(USceneComponent* InAudioComponent, const FMTWPSwitchDefinition_Base& InRtpcDefinition) {}
	virtual void SetRtpc(USceneComponent* InAudioComponent, const FMTWPRtpcDefenition_Base& InRtpcDefinition) {}
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


UCLASS(Blueprintable, BlueprintType)
class MTWP_API UMTWPAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION()
	template <typename API>
	API::AudioComponent* PlaySound(API::AudioEvent* InAudioEvent, const FMTWPAudioCreationParams InCreationParams = FMTWPAudioCreationParams(), FMTWPAudioPlaybackParams InPlaybackParams = FMTWPAudioPlaybackParams(),
		API::SwitchDefenition InSwitchDefenition = API::SwitchDefenition(), TArray<API::RtpcDefenition> RtpcDefenitions = TArray< API::RtpcDefenition>()) {}

//protected:

	// For throttling needs
	//TMap<uint32_t, float> EventTimingsMap;
};
