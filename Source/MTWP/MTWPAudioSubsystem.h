#pragma once

#include <CoreMinimal.h>
#include <Subsystems/GameInstanceSubsystem.h>

#include <AkComponent.h>
#include <AkAudioEvent.h>
#include <AkSwitchValue.h>
#include <AkRtpc.h>
#include <AkGameplayStatics.h>

#include "MTWPAudioSubsystem.generated.h"


UCLASS(BlueprintType, Blueprintable)
class MTWP_API UMTWPAudioInstance : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "PlaybackParameters")
	TObjectPtr<UMTWPPlaybackContent> ActivePlaybackContent;

	UPROPERTY(BlueprintReadOnly, Category = "PlaybackParameters")
	TArray<TObjectPtr<UMTWPPlaybackParameter>> ActivePlaybackParameters;

public:

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	virtual inline bool IsValid() const { return true; }

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	virtual void Play() {};

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	virtual void Stop() {};

	//UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	//virtual void Pause() {};

protected:

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	void UpdatePlaybackContent(UMTWPPlaybackContent* UpdatingPlaybackContent) {}

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	void UpdatePlaybackParameter(UMTWPPlaybackParameter* UpdatingPlaybackParameter) {}
};


UCLASS(BlueprintType, Blueprintable)
class MTWP_API UMTWPPlaybackContent : public UObject
{
    GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "PlaybackContent")
	const bool bCrossWorld = false;

public:
	inline virtual bool IsValid() const { return true; }
};




class MTWP_API UMTWPPlaybackParameter : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "PlaybackParameter")
	bool IsValid() const { return true; }

protected:

	UPROPERTY(BlueprintReadOnly, Category = "PlaybackParameter")
	TObjectPtr<UMTWPAudioInstance> AudioInstance;

};

UCLASS(BlueprintType, Blueprintable)
class MTWP_API UMTWPPlaybackParameterNumeric : public UMTWPPlaybackParameter
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "PlaybackParameter")
	virtual inline float GetValue() const { return Value; }

	UFUNCTION(BlueprintCallable, Category = "PlaybackParameter")
	virtual SetValue(const float NewValue);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "PlaybackParameter")
	float Value = 0;
};

UCLASS(BlueprintType, Blueprintable)
class MTWP_API UMTWPPlaybackParameterString : public UMTWPPlaybackParameter
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "PlaybackParameter")
	virtual inline FString GetValue() const { return Value; }

	UFUNCTION(BlueprintCallable, Category = "PlaybackParameter")
	virtual void SetValue(const FString& NewValue);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "PlaybackParameter")
	FString Value;
};

UCLASS(BlueprintType, Blueprintable)
class MTWP_API UMTWPPlaybackParameterBool : public UMTWPPlaybackParameter
{
    GENERATED_BODY()

public:
        UFUNCTION(BlueprintCallable, Category = "PlaybackParameter")
		virtual inline bool GetValue() const { return Value; }

		UFUNCTION(BlueprintCallable, Category = "PlaybackParameter")
		virtual void SetValue(const bool NewValue);

protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "PlaybackParameter")
	bool Value;
};





UCLASS(Abstact, BlueprintType, Blueprintable)
class MTWP_API UMTWPAudioInterface : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "AudioInterface")
	virtual UMTWPAudioInstance* CreateAudioInstance2D(UMTWPPlaybackContent* PlaybackContent, TArray<UMTWPPlaybackParameter*>& AudioParameters = TArray<UMTWPPlaybackParameter*>()) = 0;

	UFUNCTION(BlueprintCallable, Category = "AudioInterface")
	virtual UMTWPAudioInstance* CreateAudioInstanceAtLocation(UMTWPPlaybackContent* PlaybackContent, TArray<UMTWPPlaybackParameter*>& AudioParameters, const FVector& Location, const FRotator& Orientation = FRotator(ForceInit)) = 0;

	UFUNCTION(BlueprintCallable, Category = "AudioInterface")
	virtual UMTWPAudioInstance* CreateAudioInstanceAttached(UMTWPPlaybackContent* PlaybackContent, TArray<UMTWPPlaybackParameter*>& AudioParameters, USceneComponent* AttachComponent, FName AttachPointName = NAME_None, const FVector& Location = FVector(ForceInit), const FRotator& Orientation = FRotator(ForceInit), EAttachmentRule AttachmentRule = EAttachmentRule::KeepRelative) = 0;
};


UCLASS(Blueprintable, BlueprintType)
class MTWP_API UMTWPAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
};
