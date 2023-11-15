#include "MTWPAudioInterface_WWise.h"

#include <AkGameplayStatics.h>
#include <AkComponent.h>
#include <AkAudioEvent.h>
#include <AkSwitchValue.h>
#include <AkAudioDevice.h>

#include <Engine/World.h>

#include <Kismet/GameplayStatics.h>

#include "MTWPAudioInterface_WWise.h"


void UMTWPAudioInstance_WWise::Play()
{
	//if (!!!IsValid()/*TODO: || !!!IsPlaybackAllowed()*/ || !!!FAkAudioDevice::Get())
	//{
	//	// TODO: Add warning
	//	return;
	//}
	
	//if (FAkAudioDevice::Get()->IsPlayingIDActive(ActiveEventID, PlayingID))
	//{
		// TODO: Add resume functionality
		//
		//if (/*TODO: Check is paused*/)
		//{
		//	FAkAudioDevice::Get()->ExecuteActionOnPlayingID(AkActionOnEventType::Resume, PlayingID,
		//		ActivePlaybackParameters.FadeInMsec, GetActivePlaybackParameters<FMTWPPlaybackParameters_WWise>().FadeInCurveType);
		//}
		//else
		//{
		//	// TODO: Add warning
		//}
	//	return;
	//}
	//auto ActiveWWisePBP = Cast<UMTWPPlaybackParameters_WWise>(ActivePlaybackParameters);
	//if (!!!(ActiveWWisePBP && ActiveWWisePBP->IsValid()))
	//{
	//	// TODO: Add warning
	//	return;
	//}

	//if (!!!(ActiveWWisePBP->Event.IsResolved() && ActiveWWisePBP->IsValid()))
	//{
	//	// TODO: Add warning
	//	return;
	//}

	//if (!!!Component.IsResolved())
	//{
	//	// TODO: Add warning
	//	return;
	//}

	//PlayingID = Component->PostAkEvent(WWisePBP->Event);
	//
	//ActiveEventID = ActiveWWisePBP->Event->GetShortID();

	//if (!!!PlayingID)
	//{
	//	// TODO: Add warning
	//}
	//else
	//{
	//	// TODO: Exeute OnsSart events if there are some
	//	// TODO: Bind OnFinish ???
	//}
}

void UMTWPAudioInstance_WWise::Stop()
{
	//if (!!!IsValid())
	//{
	//	// TODO: Add warning
	//}
	//
	//auto ActiveWWisePBP = Cast<UMTWPPlaybackParameters_WWise>(ActivePlaybackParameters);
	//FAkAudioDevice::Get()->ExecuteActionOnPlayingID(AkActionOnEventType::Stop, PlayingID,
	//	(ActiveWWisePBP && ActiveWWisePBP->IsValid()) ? ActiveWWisePBP->FadeOutMsec : 0,
	//	(ActiveWWisePBP && ActiveWWisePBP->IsValid()) ? ActiveWWisePBP->FadeOutCurveType : EAkCurveInterpolation::Linear);
}

void UMTWPAudioInstance_WWise::OnPlaybackParameterChanged(const FName& Name)
{
	if (auto EventParam = Cast<UMTWPPareterEvent_WWise>(GetPlaybackParameter(Name)))
	{
		UpdateEvent(EventParam);
	}
	else if (auto RTPCParam = Cast<UMTWParameterPRTPC_WWise>(GetPlaybackParameter(Name)))
	{
		UpdateRTPC(RTPCParam);
	}
	else if (auto SwitchParam = Cast<UMTWParameterSwitch_WWise>(GetPlaybackParameter(Name)))
    {
        UpdateSwitch(SwitchParam);
    }
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInstance_WWise::OnPlaybackParameterChanged: Playback parameter is not valid"));
	}
}

void UMTWPAudioInstance_WWise::UpdateEvent(UMTWPPareterEvent_WWise* EventParam)
{
	//if (!!!GetWorld() || !!!FAkAudioDevice::Get())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInstance_WWise::UpdateEvent: World or AudioDevice is not valid"));
	//	return;
	//}

	//if (IsValid(EventParam))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInstance_WWise::UpdateEvent: EventParam is not valid"));
	//       return;
	//}

	//Component->SetAutoDestroy(false);
	//// TODO: Add curcvetype and interpolation time
	//UAkGameplayStatics::ExecuteActionOnPlayingID, PlayingID, AkActionOnEventType::Stop, 0, EAkCurveInterpolation::Linear);

}

void UMTWPAudioInstance_WWise::UpdateRTPC(UMTWParameterPRTPC_WWise* RTPCParam)
{
if (!!!IsValid(RTPCParam) || !!!IsValid(RTPCParam->RtpcObject))
    {
        UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInstance_WWise::UpdateRTPC: RTPCParam or RTPCObject is not valid"));
        return;
    }

    auto Result = FAkAudioDevice::Get()->SetRTPCValueByPlayingID(RTPCParam->RtpcObject->GetShortID(), RTPCParam->Value, PlayingID, RTPCParam->InterpolationTimeMs);

    if (Result != AK_Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInstance_WWise::UpdateRTPC: RTPC could not be set for sound 2D"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("MTWPAudioInstance_WWise::UpdateRTPC: RTPC set for sound 2D"));
    }
}

void UMTWPAudioInstance_WWise::UpdateSwitch(UMTWParameterSwitch_WWise* SwitchParam)
{
	if (!!!IsValid(SwitchParam) || !!!IsValid(SwitchParam->SwitchValue))
    {
        UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInstance_WWise::UpdateSwitch: SwitchParam or SwitchValue is not valid"));
        return;
    }

	if (IsValid(Component))
	{
		Component->SetSwitch(SwitchParam->SwitchValue, "", "");
	}
	else
	{
		// TODO: Add interpolation settings
		//UAkGameplayStatics::ExecuteActionOnPlayingID, PlayingID, AkActionOnEventType::Stop, 0, EAkCurveInterpolation::Linear);
	}
}

UMTWPAudioInstance* UMTWPAudioInterface_WWise::CreateAudioInstance2D(UMTWPAudioEntity* InEntity, bool bInPersistent)
{
	// TODO: Add playback validation
	//if (!!!IsPlaybackAllowed())
	//{
	//	return UMTWPAudioSystem_WWise::AudioInstanceDefinition();
	//}


	if (!!!GetWorld() || !!!FAkAudioDevice::Get())
	{
		return nullptr;
	};

	auto InEntity_WWise = Cast<UMTWPAudioEntity_WWise>(InEntity);
	if (!!!IsValid(InEntity_WWise))
	{
		return nullptr;
	}

	auto InEventObject = InEntity_WWise->Event->EventObject;
	if (!!!IsValid(InEventObject))
	{
        return nullptr;
    }

	UMTWPAudioInstance_WWise* AudioInstance = NewObject<UMTWPAudioInstance_WWise>(bInPersistent ? Cast<UObject>(GetTransientPackage()) : GetWorld()->GetWorldSettings());
	
	if (!!!IsValid(AudioInstance))
    {
        return nullptr;
    }

	// TODO: Learn more about spatial sound in WWise
	//FAkAudioDevice::GetSpatialAudioListener();

	// TODO: Need to contain 2D-sound listner somewhere.
	AudioInstance->Component = *FAkAudioDevice::Get()->GetDefaultListeners().begin();

	if (!!!IsValid(AudioInstance->Component))
    {
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: AudioComponent is not valid"));
        return nullptr;
    }

	// TODO: Learn more about callbacks
	AudioInstance->PlayingID =
		FAkAudioDevice::Get()->PostAkAudioEventOnComponent(
            InEntity_WWise->Event->EventObject,
			AudioInstance->Component,
			0U,
			nullptr,
			nullptr,
			!!!bInPersistent
        );
		
	if (!!!AudioInstance->PlayingID)
	{
        UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: PlayingID is not valid"));
        return nullptr;
    }
	
	for (auto RTPC : InEntity_WWise->RTPCs)
	{
		if (!!!RTPC || !!!RTPC->RtpcObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: RTPC is not valid"));
            continue;
        }

		auto Result = FAkAudioDevice::Get()->SetRTPCValueByPlayingID(RTPC->RtpcObject->GetShortID(), RTPC->Value, AudioInstance->PlayingID, RTPC->InterpolationTimeMs);

		if (Result != AK_Success)
		{
            UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: RTPC could not be set for sound 2D"));
        }
		else
		{
            UE_LOG(LogTemp, Log, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: RTPC set for sound 2D"));
        }

		AudioInstance->AddPlaybackParameter(RTPC);
	}

	for (auto& Switch : InEntity_WWise->SwitchesSet)
	{
		if (!!!Switch || !!!Switch->IsValidValue(Switch->GetValue()))
		{
			UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: Switch is not valid"));
            continue;
        }

		AudioInstance->AddPlaybackParameter(Switch);
		AudioInstance->Component->SetSwitch(Switch->SwitchValue, "", "");

		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: switch temporary could not be set for sound 2D"));
	}

	AudioInstance->PlaybackPositioning = EMTWPAudioPlaybackPositioning::Sound2D;

	return AudioInstance;
}