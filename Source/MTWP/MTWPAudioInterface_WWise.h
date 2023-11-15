#pragma once

#include "CoreMinimal.h"

#include "MTWPAudioSubsystem.h"

#include <AkRtpc.h>

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

	UPROPERTY(EditDefaultsOnly, Category = "AudioInstance | WWise")
	TObjectPtr<class UAkComponent> Component = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "AudioInstance | WWise")
	int PlayingID = 0;

protected:

	virtual void OnPlaybackParameterChanged(const FName& Name);

private:

	void UpdateEvent(UMTWPPareterEvent_WWise* EventParam);

	void UpdateRTPC(UMTWParameterPRTPC_WWise* RTPCParam);

	void UpdateSwitch(UMTWParameterSwitch_WWise* SwitchParam);


	friend UMTWPAudioInterface_WWise;
};


UCLASS(BlueprintType, Blueprintable)
class MTWP_API UMTWPPareterEvent_WWise : public UMTWPPlaybackParameterObject
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<class UAkAudioEvent> EventObject = nullptr;

	UPROPERTY()
	TObjectPtr<class UAkAudioEvent> PreviousEventObject = nullptr;

protected:

	virtual inline UObject* GetValue() const override { return Cast<UObject>(EventObject); }

	virtual inline UObject* GetPreviousValue() const override { return Cast<UObject>(PreviousEventObject); }

	friend UMTWPAudioInterface_WWise;
	friend UMTWPAudioInstance_WWise;
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
class MTWP_API UMTWParameterSwitch_WWise : public UMTWPPlaybackParameterObject
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FString SwitchGroup;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UAkSwitchValue> SwitchValue = nullptr;

	UPROPERTY()
	TObjectPtr<class UAkSwitchValue> PreviousSwitchValue = nullptr;


	UPROPERTY(EditDefaultsOnly)
	uint32 InterpolationTimeMs = 0;

	virtual inline UObject* GetValue() const override { return Cast<UObject>(SwitchValue); }

	virtual inline UObject* GetPreviousValue() const override { return Cast<UObject>(PreviousSwitchValue); }

	friend UMTWPAudioInterface_WWise;
	friend UMTWPAudioInstance_WWise;

	friend uint32 GetTypeHash(const UMTWParameterSwitch_WWise& Switch);
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MTWP_API UMTWPAudioEntity_WWise : public UMTWPAudioEntity
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UMTWPPareterEvent_WWise> Event;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UMTWParameterPRTPC_WWise*> RTPCs;
	
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UMTWParameterSwitch_WWise*> SwitchesSet;

	friend UMTWPAudioInterface_WWise;
	friend UMTWPAudioInstance_WWise;
};


inline uint32 GetTypeHash(const UMTWParameterSwitch_WWise& Switch)
{
    return GetTypeHash(Switch.SwitchGroup);
}

inline uint32 GetTypeHash(const UMTWParameterPRTPC_WWise& Rtpc)
{
	return GetTypeHash(Rtpc.RtpcObject->GetShortID());
}


UCLASS(Blueprintable, BlueprintType)
class MTWP_API UMTWPAudioInterface_WWise : public UMTWPAudioInterface
{
	GENERATED_BODY()

public:

	virtual UMTWPAudioInstance* CreateAudioInstance2D(UMTWPAudioEntity* InEntity, bool bInPersistent = false) override;

	virtual UMTWPAudioInstance* CreateAudioInstanceAtLocation(UMTWPAudioEntity* InEntity, const FVector& Location, const FRotator& Orientation = FRotator(ForceInit)) override { return nullptr; }

	//virtual UMTWPAudioInstance* CreateAudioInstanceAttached(const UMTWPPlaybackParameters* AudioEntity, USceneComponent* AttachComponent, FName AttachPointName = NAME_None, const FVector& Location = FVector(ForceInit), const FRotator& Orientation = FRotator(ForceInit), EAttachmentRule AttachmentRule = EAttachmentRule::KeepRelative) override;
};
