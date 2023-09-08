#include "MTWPAudioSubsystem.h"

#include <AkComponent.h>
#include <AkAudioEvent.h>
#include <AkGameplayStatics.h>
#include <AKRtpc.h>

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

    if (!FMath::IsNearlyZero(InPlaybackParams.CooldownSeconds))
    {
        auto Now = GetWorld()->GetTimeSeconds();
        auto CurrentID = InAudioEvent->GetShortID();

        if (EventTimingsMap.Contains(CurrentID))
        {
            auto  PrevTimings = EventTimingsMap[CurrentID];
            auto Gap = FMath::Abs(Now - PrevTimings);
            if (Gap < InPlaybackParams.CooldownSeconds)
            {
                return nullptr;
            }
        }

        EventTimingsMap.Emplace(CurrentID, Now);
    }

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

    // We basicaly declare that AudioComponent are killed when the
    // when the playback is finished.
    // It's better to keep an eye on every one :)
    ResultAudioComponent->SetAutoDestroy(true);

    // Switch default
    if (InPlaybackParams.SwitchValue.IsResolved())
    {
        ResultAudioComponent->SetSwitch(InPlaybackParams.SwitchValue, InPlaybackParams.SwitchGroupName, "");
    }

    // Rtpc default
    for (auto& RtpcDefinition : InPlaybackParams.RtpcDefinitions)
    {
        if (RtpcDefinition.Rtpc)
        {
            auto CastedValue = RtpcDefinition.Value / RtpcDefinition.MaxGameValue * RtpcDefinition.DeclaredMaxSoundEngnValue;
            ResultAudioComponent->SetRTPCValue(RtpcDefinition.Rtpc, CastedValue, RtpcDefinition.InterpolationSeconds, "");
        }
    }

    if (auto Result = ResultAudioComponent->PostAkEvent(InAudioEvent); Result == AK_INVALID_PLAYING_ID)
    {
        return nullptr;
    }

    return ResultAudioComponent;
}
