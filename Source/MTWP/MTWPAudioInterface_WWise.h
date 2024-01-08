#pragma once

#include "CoreMinimal.h"

#include "MTWPAudioSubsystem.h"

#include <AkRtpc.h>
#include <AkGameplayTypes.h>

#include "MTWPAudioInterface_WWise.generated.h"


UCLASS(BlueprintType, Blueprintable)
class MTWP_API UMTWPAudioInstance_WWise : public UMTWPAudioInstance
{
	GENERATED_BODY()

public:

	//UFUNCTION(BlueprintCallable, Category = "AudioInstance | WWise")
	//virtual bool IsValid() const override;

	virtual void Play() override;

	virtual void Stop() override;

	//virtual void Pause() = 0;

protected:

	virtual void BeginDestroy() override;

protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UAkAudioEvent> Event;

	UPROPERTY(EditDefaultsOnly)
	int TransitionDurationMs = 0;

	UPROPERTY(EditDefaultsOnly)
	EAkCurveInterpolation CurveInterpolation = EAkCurveInterpolation::Linear;

// protected:
public:

	UPROPERTY(EditDefaultsOnly, Category = "AudioInstance | WWise")
	TObjectPtr<class UAkComponent> Component = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "AudioInstance | WWise")
	int PlayingID = 0;

protected:

	virtual bool UpdateParameterNumeric(UMTWPPlaybackParameterNumeric* ParameterNumeric, float InValue) override;

	virtual bool UpdateParameterString(UMTWPPlaybackParameterString* ParameterString, const FString& InValue) override;

protected:

	bool bPersistent = false;

	friend UMTWPAudioInterface_WWise;
};

UCLASS(BlueprintType, Blueprintable)
class MTWP_API UMTWParameterPRTPC_WWise : public UMTWPPlaybackParameterNumeric
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UAkRtpc> RtpcObject = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int InterpolationTimeMs = 0;


	friend UMTWPAudioInterface_WWise;
	friend UMTWPAudioInstance_WWise;

	friend uint32 GetTypeHash(const UMTWParameterPRTPC_WWise& Rtpc);
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MTWP_API UMTWParameterSwitchGroup_WWise : public UMTWPPlaybackParameterString
{
	GENERATED_BODY()

	friend UMTWPAudioInterface_WWise;
	friend UMTWPAudioInstance_WWise;
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MTWP_API UMTWPAudioEntity_WWise : public UMTWPAudioEntity
{
	GENERATED_BODY()
	
//protected:
public:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UAkAudioEvent> Event;

	UPROPERTY(EditDefaultsOnly)
	int TransitionDurationMs = 0;

	UPROPERTY(EditDefaultsOnly)
	EAkCurveInterpolation CurveInterpolation = EAkCurveInterpolation::Linear;
	
protected:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UMTWParameterPRTPC_WWise*> RTPCs;
	
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UMTWParameterSwitchGroup_WWise*> Switches;

	friend UMTWPAudioInterface_WWise;
	friend UMTWPAudioInstance_WWise;
};

UCLASS(Blueprintable, BlueprintType)
class MTWP_API UMTWPAudioInterface_WWise : public UMTWPAudioInterface
{
	GENERATED_BODY()

public:

	virtual UMTWPAudioInstance* CreateAudioInstance2D(UMTWPAudioEntity* InEntity, bool bInPersistent = false) override;

	virtual UMTWPAudioInstance* CreateAudioInstanceAtLocation(UMTWPAudioEntity* InEntity, const FVector& Location, const FRotator& Orientation = FRotator(ForceInit)) override;

	virtual UMTWPAudioInstance* CreateAudioInstanceAttached(UMTWPAudioEntity* InEntity, USceneComponent* AttachComponent, FName AttachPointName = NAME_None, const FVector& Location = FVector(ForceInit), const FRotator& Orientation = FRotator(ForceInit), const EAttachmentRule& InAttachmentRule = EAttachmentRule::KeepRelative) override;

protected:

	virtual bool IsValidEntity(UMTWPAudioEntity* InEntity) const override;

protected:

	void SetupParams(const UMTWPAudioEntity_WWise& InEntityChecked, UMTWPAudioInstance_WWise& InInstanceChecked);
};
