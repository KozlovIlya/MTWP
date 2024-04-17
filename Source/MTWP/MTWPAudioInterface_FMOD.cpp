// Fill out your copyright notice in the Description page of Project Settings.


#include "MTWPAudioInterface_FMOD.h"

void UMTWPAudioInstance_FMOD::Play()
{
    //if (IsValid(AudioComponent) && !!!AudioComponent->IsPlaying() &&
    //    (PlayingStatus == EMTWPPlayingStatus::Init || PlayingStatus == EMTWPPlayingStatus::Paused))
    //{
    //    AudioComponent->Play();
    //    PlayingStatus = EMTWPPlayingStatus::Playing;
    //}
}

void UMTWPAudioInstance_FMOD::Stop()
{
    //if (PlayingStatus == EMTWPPlayingStatus::Init || PlayingStatus == EMTWPPlayingStatus::Paused)
    //{
    //    if (IsValid(AudioComponent) && AudioComponent->IsPlaying()) 
    //    {
    //        AudioComponent->Stop();
    //    }
    //    else if (EventInstance.IsValid() && EventInstance->set)
    //    {

    //    }
    //}
}

UMTWPAudioInstance* UMTWPAudioInterface_FMOD::CreateAudioInstance2D(UMTWPAudioEntity* InEntity, bool bInPersistent)
{
    return nullptr;
}

UMTWPAudioInstance* UMTWPAudioInterface_FMOD::CreateAudioInstanceAtLocation(UMTWPAudioEntity* InEntity, const FVector& Location, const FRotator& Orientation)
{
    return nullptr;
}

UMTWPAudioInstance* UMTWPAudioInterface_FMOD::CreateAudioInstanceAttached(UMTWPAudioEntity* InEntity, USceneComponent* AttachComponent, FName AttachPointName, const FVector& Location, const FRotator& Orientation, const EAttachmentRule& InAttachmentRule)
{
    return nullptr;
}

bool UMTWPAudioInterface_FMOD::IsValidEntity(UMTWPAudioEntity* InEntity) const
{
    return false;
}

UMTWPAudioInterface* UMTWPAudioEntity_FMOD::CreateAudioInterface() const
{
    return NewObject<UMTWPAudioInterface_FMOD>();
}
