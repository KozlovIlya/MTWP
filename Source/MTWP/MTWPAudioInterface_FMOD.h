#pragma once

#include "CoreMinimal.h"

#include "MTWPAudioSubsystem.h"

#include "FMODAudioComponent.h"

#include "MTWPAudioInterface_FMOD.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MTWP_API UMTWPAudioInstance_FMOD : public UMTWPAudioInstance
{
    GENERATED_BODY()

public:

    virtual void Play() override;

    virtual void Stop() override;

    //virtual void Pause() override;

protected:
    
    UPROPERTY()
    TObjectPtr<UFMODAudioComponent> AudioComponent;
    
    TUniquePtr<FMOD::Studio::EventInstance> EventInstance;
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MTWP_API UMTWParameterNumeric_FMOD : public UMTWPPlaybackParameterNumeric
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MTWP_API UMTWParameterString_FMOD : public UMTWPPlaybackParameterString
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MTWP_API UMTWPAudioEntity_FMOD : public UMTWPAudioEntity
{
    GENERATED_BODY()

protected:

    UPROPERTY(EditDefaultsOnly, Instanced)
    TObjectPtr<UFMODEvent> Event;

    UPROPERTY(EditDefaultsOnly, Instanced)
    TArray<UMTWParameterNumeric_FMOD*> NumericParameters;

    UPROPERTY(EditDefaultsOnly, Instanced)
    TArray<UMTWParameterString_FMOD*> StringParameters;

protected:

    virtual UMTWPAudioInterface* CreateAudioInterface() const override;
};

UCLASS(Blueprintable, BlueprintType)
class MTWP_API UMTWPAudioInterface_FMOD : public UMTWPAudioInterface
{
    GENERATED_BODY()

public:

    virtual UMTWPAudioInstance* CreateAudioInstance2D(UMTWPAudioEntity* InEntity, bool bInPersistent = false) override;

    virtual UMTWPAudioInstance* CreateAudioInstanceAtLocation(UMTWPAudioEntity* InEntity, const FVector& Location, const FRotator& Orientation = FRotator(ForceInit)) override;

    virtual UMTWPAudioInstance* CreateAudioInstanceAttached(UMTWPAudioEntity* InEntity, USceneComponent* AttachComponent, FName AttachPointName = NAME_None, const FVector& Location = FVector(ForceInit), const FRotator& Orientation = FRotator(ForceInit), const EAttachmentRule& InAttachmentRule = EAttachmentRule::KeepRelative) override;

protected:

    virtual bool IsValidEntity(UMTWPAudioEntity* InEntity) const override;

};