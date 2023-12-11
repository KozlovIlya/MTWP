#pragma once

#include <CoreMinimal.h>
#include <Subsystems/GameInstanceSubsystem.h>

#include "MTWPAudioSubsystem.generated.h"


inline FString ToString(float Value)
{
    return FString::SanitizeFloat(Value);
}

inline FString ToString(const FString& Value)
{
    return Value;
}

inline FString ToString(bool Value)
{
    return Value ? TEXT("true") : TEXT("false");
}

inline FString ToString(UObject* Value)
{
    return Value ? Value->GetName() : TEXT("nullptr");
}

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MTWP_API UMTWPPlaybackParameter : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	FName GetName() const { return Name; }

protected:

	UPROPERTY(EditDefaultsOnly)
	FName Name;

	friend UMTWPAudioInstance;
	friend UMTWPAudioInterface;
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MTWP_API UMTWPPlaybackParameterNumeric : public UMTWPPlaybackParameter
{
	GENERATED_BODY()

	using GetValueType = float;
	using SetValueType = float;

	const float DefaultValue = 0.f;

public:
	
	UFUNCTION(BlueprintCallable, Category = "PlaybackParameter")
	virtual inline bool IsValidValue(const float InValue) const { return true; }

	UFUNCTION(BlueprintCallable, Category = "PlaybackParameter")
	virtual inline float GetValue() const { return Value; }

	UFUNCTION(BlueprintCallable, Category = "Playback Parameter")
	virtual inline float GetPreviousValue() const { return PreviousValue; }

protected:

	UFUNCTION(BlueprintCallable, Category = "Playback Parameter")
	virtual inline void SetValue(float InValue) { Value = InValue; }

	UFUNCTION(BlueprintCallable, Category = "Playback Parameter")
	virtual inline void SetPreviousValue(float InPreviousValue) { PreviousValue = InPreviousValue; }

protected:

	UPROPERTY(EditDefaultsOnly, Category = "PlaybackParameter")
	float Value;

	UPROPERTY(BlueprintReadOnly, Category = "PlaybackParameter")
	float PreviousValue;

	friend UMTWPAudioInstance;
	friend UMTWPAudioInterface;
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MTWP_API UMTWPPlaybackParameterString : public UMTWPPlaybackParameter
{
	GENERATED_BODY()

	using GetValueType = FString;
	using SetValueType = const FString&;

	const FString DefaultValue = FString();

public:

	UFUNCTION(BlueprintCallable, Category = "PlaybackParameter")
	virtual inline bool IsValidValue(const FString& InValue) const { return !!!InValue.IsEmpty(); }

	UFUNCTION(BlueprintCallable, Category = "PlaybackParameter")
	virtual inline FString GetValue() const { return Value; }

	UFUNCTION(BlueprintCallable, Category = "Playback Parameter")
	virtual inline FString GetPreviousValue() const { return PreviousValue; }

protected:

	UFUNCTION(BlueprintCallable, Category = "Playback Parameter")
	virtual inline void SetValue(const FString& InValue) { Value = InValue; }

	UFUNCTION(BlueprintCallable, Category = "Playback Parameter")
	virtual inline void SetPreviousValue(const FString& InPreviousValue) { PreviousValue = InPreviousValue; }

protected:

	UPROPERTY(EditDefaultsOnly, Category = "PlaybackParameter")
	FString Value;

	UPROPERTY(BlueprintReadOnly, Category = "PlaybackParameter")
	FString PreviousValue;

	friend UMTWPAudioInstance;
	friend UMTWPAudioInterface;
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MTWP_API UMTWPPlaybackParameterBoolean : public UMTWPPlaybackParameter
{
	GENERATED_BODY()

	using GetValueType = bool;
	using SetValueType = bool;

	const bool DefaultValue = false;

public:

	UFUNCTION(BlueprintCallable, Category = "PlaybackParameter")
	virtual inline bool IsValidValue(bool InValue) const { return true; }
	
	UFUNCTION(BlueprintCallable, Category = "Playback Parameter")
	virtual inline bool GetValue() const { return Value; }

	UFUNCTION(BlueprintCallable, Category = "Playback Parameter")
	virtual inline bool GetPreviousValue() const { return PreviousValue; }

protected:

	UFUNCTION(BlueprintCallable, Category = "Playback Parameter")
	virtual inline void SetValue(bool InValue) { Value = InValue; }

	UFUNCTION(BlueprintCallable, Category = "Playback Parameter")
	virtual inline void SetPreviousValue(bool InPreviousValue) { PreviousValue = InPreviousValue; }

protected:

	UPROPERTY(EditDefaultsOnly, Category = "PlaybackParameter")
	bool Value;

	UPROPERTY(BlueprintReadOnly, Category = "PlaybackParameter")
	bool PreviousValue;


	friend UMTWPAudioInstance;
	friend UMTWPAudioInterface;
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MTWP_API UMTWPPlaybackParameterObject : public UMTWPPlaybackParameter
{
	GENERATED_BODY()

	using GetValueType = UObject*;
	using SetValueType = UObject*;

	const UObject* DefaultValue = nullptr;

public:

	UFUNCTION(BlueprintCallable, Category = "Playback Parameter")
	virtual inline bool IsValidValue (const UObject* InValue) const { return IsValid(InValue); }

	UFUNCTION(BlueprintCallable, Category = "Playback Parameter")
	virtual UObject* GetValue() const { return nullptr; }

	UFUNCTION(BlueprintCallable, Category = "Playback Parameter")
	virtual UObject* GetPreviousValue() const { return nullptr; }

protected:

	UFUNCTION(BlueprintCallable, Category = "Playback Parameter")
	virtual inline void SetValue(UObject* InValue) { }

	UFUNCTION(BlueprintCallable, Category = "Playback Parameter")
	virtual inline void SetPreviousValue(UObject* InPreviousValue) { }


	friend UMTWPAudioInstance;
	friend UMTWPAudioInterface;
};



UCLASS(BlueprintType, Blueprintable)
class MTWP_API UMTWPAudioInstance : public UObject
{
	GENERATED_BODY()

	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlaybackParameterChanged, const FName&, Name);

public:

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	virtual inline bool IsValidInstance() const { return true; }

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	virtual void Play() {};

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	virtual void Stop() {};

	//UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	//virtual void Pause() {};

public:

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | Numeric")
	virtual void SetParameterNumeric(const FName& Name, float Value);

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | String")
	virtual void SetParameterString(const FName& Name, const FString& Value);

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | Boolean")
	virtual void SetParameterBoolean(FName Name, bool Value);

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | Object pointer")
	virtual void SetParameterObjectPointer(const FName& Name, UObject* Object);


	UFUNCTION(BlueprintCallable, Category = "AudioInstance | Numeric")
	virtual float GetParameterNumeric(const FName& Name) const;

    UFUNCTION(BlueprintCallable, Category = "AudioInstance | String")
	virtual FString GetParameterString(const FName& Name) const;

    UFUNCTION(BlueprintCallable, Category = "AudioInstance | Boolean")
	virtual bool GetParameterBoolean(const FName& Name) const;

    UFUNCTION(BlueprintCallable, Category = "AudioInstance | Object pointer")
	virtual UObject* GetParameterObjectPointer(const FName& Name) const;


	UFUNCTION(BlueprintCallable, Category = "AudioInstance | Numeric")
	virtual float GetPreviousParameterNumeric(const FName& Name) const;

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | String")
	virtual FString GetPreviousParameterString(const FName& Name) const;

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | Boolean")
	virtual bool GetPreviousParameterBoolean(const FName& Name) const;

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | Object pointer")
	virtual UObject* GetPreviousParameterObjectPointer(const FName& Name) const;


//public:
//
//	template<typename T>
//	void SetParameter(const FName& Name, T::SetValueType Value);
//
//	template<typename T>
//	T::GetValueType GetParameter(const FName& Name) const;
//
//	template<typename T>
//	T::GetValueType GetPreviousParameter(const FName& Name) const;
	
public:

	UPROPERTY(BlueprintAssignable, Category = "AudioInstance")
	FPlaybackParameterChanged PlaybackParameterChanged;

protected:

	UFUNCTION(Category = "AudioInstance")
	virtual void OnPlaybackParameterChanged(const FName& Name) {}

protected:

	inline virtual void PostInitProperties() override
	{
		Super::PostInitProperties();
		PlaybackParameterChanged.AddDynamic(this, &UMTWPAudioInstance::OnPlaybackParameterChanged);
	}

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	bool AddPlaybackParameter(UMTWPPlaybackParameter* InPlaybackParameter);

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	UMTWPPlaybackParameter* GetPlaybackParameter(const FName& Name);

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | Const")
	inline UMTWPPlaybackParameter* GetPlaybackParameterConst(const FName& Name) const
	{
		return const_cast<UMTWPPlaybackParameter*>(const_cast<UMTWPAudioInstance*>(this)->GetPlaybackParameter(Name));
	}

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Playback Parameters")
	TMap<FName, int> PlaybackParametersIndexMap;

	UPROPERTY(BlueprintReadWrite, Category = "Playback Parameters")
	TArray<UMTWPPlaybackParameter*> PlaybackParameters;


	friend UMTWPAudioInterface;
};


UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MTWP_API UMTWPAudioEntity : public UObject
{
    GENERATED_BODY()

	friend UMTWPAudioInstance;
};



UCLASS(BlueprintType, Blueprintable)
class MTWP_API UMTWPAudioInterface : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "AudioInterface")
	virtual UMTWPAudioInstance* CreateAudioInstance2D(UMTWPAudioEntity* InEntity, bool bInPersistent = false) { return nullptr; }

	UFUNCTION(BlueprintCallable, Category = "AudioInterface")
	virtual UMTWPAudioInstance* CreateAudioInstanceAtLocation(UMTWPAudioEntity* InEntity, const FVector& Location, const FRotator& Orientation = FRotator(ForceInit)) { return nullptr; }

	//UFUNCTION(BlueprintCallable, Category = "AudioInterface")
	//virtual UMTWPAudioInstance* CreateAudioInstanceAttached(UMTWPPlaybackContent* PlaybackContent, TArray<UMTWPPlaybackParameter*>& AudioParameters, USceneComponent* AttachComponent, FName AttachPointName = NAME_None, const FVector& Location = FVector(ForceInit), const FRotator& Orientation = FRotator(ForceInit), EAttachmentRule AttachmentRule = EAttachmentRule::KeepRelative) = 0;
};



UCLASS(Blueprintable, BlueprintType)
class MTWP_API UMTWPAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection);

public:

	// TODO: Should be contained in array
	class UMTWPAudioInterface_WWise* WWiseAudioInterface;
};
