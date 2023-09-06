#include "MTWPAudioSubsystem.h"

#include <AkComponent.h>
#include <AkAudioEvent.h>
#include <AkGameplayStatics.h>

UAkComponent* UMTWPAudioSubsystem::PlaySound(UAkAudioEvent* InAudioEvent, const FMTWPAudioCreationParams InCreationParams, FMTWPAudioPlaybackParams InPlaybackParams)
{
    if (!GetWorld() || !IsValid(InAudioEvent))
    {
        return nullptr;
    }

    if (!GetWorld()->AllowAudioPlayback())
    {
        return nullptr;
    }


    auto Now = GetWorld()->GetTimeSeconds();
    auto CurrentID = InAudioEvent->GetShortID();

    if (EventTimingsMap.Contains(CurrentID))
    {
        auto  a = EventTimingsMap[CurrentID];
        auto Gap = FMath::Abs(Now - a);
        if (Gap < InPlaybackParams.CooldownSeconds)
        {    
            return nullptr;
        }
    }

    EventTimingsMap.Emplace(CurrentID, Now);

    UAkComponent* ResultAudioComponent = nullptr;
    bool bComponentCreated = true;

    // 2D
    if (InCreationParams.IsPlaying2D)
    {
        if (!IsValid(GetWorld()->GetFirstPlayerController()))
        {
            return nullptr;
        }
        ResultAudioComponent = UAkGameplayStatics::GetAkComponent(GetWorld()->GetFirstPlayerController()->GetRootComponent(), bComponentCreated);
    }
    // Attached
    else if (IsValid(InCreationParams.SceneComponent))
    {
        ResultAudioComponent = UAkGameplayStatics::GetAkComponent(InCreationParams.SceneComponent, bComponentCreated, InCreationParams.AttachPointName, InCreationParams.Location);
        
    }
    // At location
    else
    {
        ResultAudioComponent = UAkGameplayStatics::SpawnAkComponentAtLocation(GetWorld(), InAudioEvent, InCreationParams.Location, InCreationParams.Rotation, false, "", true);
    }


    if (!IsValid(ResultAudioComponent))
    {
        return nullptr;
    }

    ResultAudioComponent->SetAutoDestroy(true);

    if (InPlaybackParams.SwitchValue.IsResolved())
    {
        ResultAudioComponent->SetSwitch(InPlaybackParams.SwitchValue, InPlaybackParams.SwitchGroupName, "");
    }

    auto Result = ResultAudioComponent->PostAkEvent(InAudioEvent);

    return ResultAudioComponent;
}
