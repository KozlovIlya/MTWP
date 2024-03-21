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


UENUM(BlueprintType)
enum class EMTWPPlaybackParameterType : uint8
{
	None,
	Numeric,
    String,
    Boolean,
    ObjectPointer
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MTWP_API UMTWPPlaybackParameter : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	FName GetName() const { return Name; }

	UFUNCTION(BlueprintCallable)
	virtual EMTWPPlaybackParameterType GetType() const { return EMTWPPlaybackParameterType::None; }

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

	virtual EMTWPPlaybackParameterType GetType() const override { return EMTWPPlaybackParameterType::Numeric; }

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

	virtual EMTWPPlaybackParameterType GetType() const override { return EMTWPPlaybackParameterType::String; }

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

	virtual EMTWPPlaybackParameterType GetType() const override { return EMTWPPlaybackParameterType::Boolean; }

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
class MTWP_API UMTWPPlaybackParameterObjectPointer : public UMTWPPlaybackParameter
{
	GENERATED_BODY()

	using GetValueType = UObject*;
	using SetValueType = UObject*;

	const UObject* DefaultValue = nullptr;

public:

	virtual EMTWPPlaybackParameterType GetType() const override { return EMTWPPlaybackParameterType::ObjectPointer; }

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

UENUM(BlueprintType)
enum class EMTWPPlayingStatus : uint8
{
	Init,
	Playing,
	Paused,
	Destroying
};

UENUM(BlueprintType)
enum class EMTWPPlayingType : uint8
{
	Flat,
	AtLocation,
	Attached
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
	virtual void Play();

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	virtual void Stop() {};

	//UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	//virtual void Pause() {};


public:

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	inline EMTWPPlayingStatus GetPlayingStatus() const { return PlayingStatus; }


	UFUNCTION(BlueprintCallable, Category = "AudioInstance | Numeric")
	void SetParameterValueNumeric(const FName& Name, float Value);

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | String")
	void SetParameterValueString(const FName& Name, const FString& Value);

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | Boolean")
	void SetParameterValueBoolean(FName Name, bool Value);

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | Object pointer")
	void SetParameterValueObjectPointer(const FName& Name, UObject* Value);


	UFUNCTION(BlueprintCallable, Category = "AudioInstance | Numeric")
	float GetParameterValueNumeric(const FName& Name) const;

    UFUNCTION(BlueprintCallable, Category = "AudioInstance | String")
	FString GetParameterValueString(const FName& Name) const;

    UFUNCTION(BlueprintCallable, Category = "AudioInstance | Boolean")
	bool GetParameterValueBoolean(const FName& Name) const;

    UFUNCTION(BlueprintCallable, Category = "AudioInstance | Object pointer")
	UObject* GetParameterValueObjectPointer(const FName& Name) const;


	UFUNCTION(BlueprintCallable, Category = "AudioInstance | Numeric")
	float GetPreviousParameterValueNumeric(const FName& Name) const;

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | String")
	FString GetPreviousParameterValueString(const FName& Name) const;

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | Boolean")
	bool GetPreviousParameterValueBoolean(const FName& Name) const;

	UFUNCTION(BlueprintCallable, Category = "AudioInstance | Object pointer")
	UObject* GetPreviousParameterValueObjectPointer(const FName& Name) const;

	
protected:

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	virtual bool UpdateParameterNumeric(UMTWPPlaybackParameterNumeric* ParameterNumeric) { return false; }

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	virtual bool UpdateParameterString(UMTWPPlaybackParameterString* ParameterString) { return false; }

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	virtual bool UpdateParameterBoolean(UMTWPPlaybackParameterBoolean* ParameterNumeric) { return false; }

	UFUNCTION(BlueprintCallable, Category = "AudioInstance")
	virtual bool UpdateParameterObjectPointer(UMTWPPlaybackParameterObjectPointer* ParameterNumeric) { return false; }

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
	virtual inline bool UpdateParameter(const FName& Name);

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

	UPROPERTY(BlueprintReadWrite, Category = "Playback Parameters")
	EMTWPPlayingStatus PlayingStatus = EMTWPPlayingStatus::Init;

	UPROPERTY(BlueprintReadWrite, Category = "Playback Parameters")
	EMTWPPlayingType PlayingType = EMTWPPlayingType::Flat;


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
	virtual UMTWPAudioInstance* CreateAudioInstance2D(UMTWPAudioEntity* InEntity, bool bInPersistent = false) { return nullptr; };

	UFUNCTION(BlueprintCallable, Category = "AudioInterface")
	virtual UMTWPAudioInstance* CreateAudioInstanceAtLocation(UMTWPAudioEntity* InEntity, const FVector& Location, const FRotator& Orientation = FRotator(ForceInit)) { return nullptr; }

	UFUNCTION(BlueprintCallable, Category = "AudioInterface")
	virtual UMTWPAudioInstance* CreateAudioInstanceAttached(UMTWPAudioEntity* InEntity, USceneComponent* AttachComponent, FName AttachPointName = NAME_None, const FVector& Location = FVector(ForceInit), const FRotator& Orientation = FRotator(ForceInit), const EAttachmentRule& InAttachmentRule = EAttachmentRule::KeepRelative) { return nullptr; }

protected:

	UFUNCTION(BlueprintCallable, Category = "AudioInterface")
	virtual bool IsValidEntity(UMTWPAudioEntity* InEntity) const { return true; }

};



UCLASS(Blueprintable, BlueprintType)
class MTWP_API UMTWPAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection);

public:

	// TODO: Should be contained in array
	UPROPERTY()
	class UMTWPAudioInterface_WWise* WWiseAudioInterface;
};
