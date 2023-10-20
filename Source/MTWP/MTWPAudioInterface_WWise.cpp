#include "MTWPAudioInterface_WWise.h"

#include <AkGameplayStatics.h>
#include <AkComponent.h>
#include <AkAudioEvent.h>
#include <AkSwitchValue.h>
#include <AkRtpc.h>

#include <Engine/World.h>

#include <Kismet/GameplayStatics.h>

#include "MTWPAudioInterface_WWise.h"


//UMTWPAudioSystem_WWise::AudioInstanceDefinition UMTWPAudioSystem_WWise::CreateAudioInstance2D(const UMTWPAudioSystem_WWise::AudioEventDefinition& InEventDifinition, const UMTWPAudioSystem_WWise::AudioCreationParamsDefinition& InAudioCreationParamsDefinition, const UMTWPAudioSystem_WWise::AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition)
//{
//	if (!!!IsPlaybackAllowed())
//	{
//		return UMTWPAudioSystem_WWise::AudioInstanceDefinition();
//	}
//	if (!!!GetWorld() || !!!IsValid(GetWorld()->GetFirstPlayerController()))
//	{
//		return UMTWPAudioSystem_WWise::AudioInstanceDefinition();
//	}
//	bool bCreated;
//	auto ResultAudioComponent = UAkGameplayStatics::GetAkComponent(GetWorld()->GetFirstPlayerController()->GetRootComponent(), bCreated);
//	UMTWPAudioSystem_WWise::AudioInstanceDefinition AudioInstanceDefinition;
//	AudioInstanceDefinition.Component = ResultAudioComponent;
//
//	return AudioInstanceDefinition;
//}
//
//UMTWPAudioSystem_WWise::AudioInstanceDefinition UMTWPAudioSystem_WWise::CreateAudioInstanceAttached(const UMTWPAudioSystem_WWise::AudioEventDefinition& InEventDifinition, const UMTWPAudioSystem_WWise::AudioCreationParamsDefinition& InAudioCreationParamsDefinition, const UMTWPAudioSystem_WWise::AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition)
//{
//	if (!!!IsPlaybackAllowed())
//	{
//		return UMTWPAudioSystem_WWise::AudioInstanceDefinition();
//	}
//
//	bool bCreated;
//	auto ResultAudioComponent = UAkGameplayStatics::GetAkComponent(InAudioCreationParamsDefinition.SceneComponent, bCreated, InAudioCreationParamsDefinition.AttachPointName, InAudioCreationParamsDefinition.Location, InAudioCreationParamsDefinition.AttachLocation);
//	if (IsValid(ResultAudioComponent) && InAudioCreationParamsDefinition.AttachLocation == EAttachLocation::KeepWorldPosition)
//	{
//		ResultAudioComponent->SetWorldRotation(InAudioCreationParamsDefinition.Rotation);
//	}
//	UMTWPAudioSystem_WWise::AudioInstanceDefinition AudioInstanceDefinition;
//	AudioInstanceDefinition.Component = ResultAudioComponent;
//
//	return AudioInstanceDefinition;
//}
//
//UMTWPAudioSystem_WWise::AudioInstanceDefinition UMTWPAudioSystem_WWise::CreateAudioInstanceAtLocation(const UMTWPAudioSystem_WWise::AudioEventDefinition& InEventDifinition, const UMTWPAudioSystem_WWise::AudioCreationParamsDefinition& InAudioCreationParamsDefinition, const UMTWPAudioSystem_WWise::AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition)
//{
//	if (!!!IsPlaybackAllowed())
//	{
//		return UMTWPAudioSystem_WWise::AudioInstanceDefinition();
//	}
//
//	bool bCreated = false;
//	auto ResultAudioComponent = UAkGameplayStatics::SpawnAkComponentAtLocation(this, InEventDifinition.Event, InAudioCreationParamsDefinition.Location, InAudioCreationParamsDefinition.Rotation, false, FString());
//	UMTWPAudioSystem_WWise::AudioInstanceDefinition AudioInstanceDefinition;
//	AudioInstanceDefinition.Component = ResultAudioComponent;
//
//	return AudioInstanceDefinition;
//}
//
//void UMTWPAudioSystem_WWise::SetPlaybackParams(UMTWPAudioSystem_WWise::AudioInstanceDefinition InAudioInstanceDefinition, const UMTWPAudioSystem_WWise::AudioPlaybackParamsDefinition InAudioPlaybackParamsDifinition)
//{
//	if (!!!InAudioInstanceDefinition.IsValid() || !!!InAudioPlaybackParamsDifinition.IsValid())
//	{
//		return;
//	}
//
//	if (InAudioPlaybackParamsDifinition.RtpcDefinition.IsValid())
//	{
//		if (IsValid(InAudioInstanceDefinition.Component))
//		{
//			InAudioInstanceDefinition.Component->SetRTPCValue(InAudioPlaybackParamsDifinition.RtpcDefinition.Rtpc, InAudioPlaybackParamsDifinition.RtpcDefinition.Value, InAudioPlaybackParamsDifinition.RtpcDefinition.InterpolationTimeMs, "");
//		}
//		else if (FAkAudioDevice::Get())
//		{
//			FAkAudioDevice::Get()->SetRTPCValueByPlayingID(InAudioPlaybackParamsDifinition.RtpcDefinition.Rtpc->GetShortID(), InAudioPlaybackParamsDifinition.RtpcDefinition.Value, InAudioInstanceDefinition.PlayingID, InAudioPlaybackParamsDifinition.RtpcDefinition.InterpolationTimeMs);
//		}
//	}
//
//	if (InAudioPlaybackParamsDifinition.SwitchDefinition.IsValid())
//	{
//		if (IsValid(InAudioInstanceDefinition.Component))
//		{
//			InAudioInstanceDefinition.Component->SetSwitch(InAudioPlaybackParamsDifinition.SwitchDefinition.Switch, InAudioPlaybackParamsDifinition.SwitchDefinition.SwitchGroup, "");
//		}
//
//		// TODO: Find the way how to set switch using PlayingID
//		// It should be quite tricky because there is no direct method for it
//		// It may be an API call but I have not found it yet
//		else if (FAkAudioDevice::Get())
//		{
//			// ...
//		}
//	}
//}

bool UMTWPAudioInstance_WWise::IsValid() const
{
	return Component.IsResolved() || FAkAudioDevice::Get()->IsPlayingIDActive(ActiveEventID, PlayingID);
}

void UMTWPAudioInstance_WWise::Play()
{
	if (!!!IsValid()/*TODO: || !!!IsPlaybackAllowed()*/ || !!!FAkAudioDevice::Get())
	{
		// TODO: Add warning
		return;
	}
	
	if (FAkAudioDevice::Get()->IsPlayingIDActive(ActiveEventID, PlayingID))
	{
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
		return;
	}
	auto ActiveWWisePBP = Cast<UMTWPPlaybackParameters_WWise>(ActivePlaybackParameters);
	if (!!!(ActiveWWisePBP && ActiveWWisePBP->IsValid()))
	{
		// TODO: Add warning
		return;
	}

	if (!!!(ActiveWWisePBP->Event.IsResolved() && ActiveWWisePBP->IsValid()))
	{
		// TODO: Add warning
		return;
	}

	if (!!!Component.IsResolved())
	{
		// TODO: Add warning
		return;
	}

	PlayingID = Component->PostAkEvent(WWisePBP->Event);
	ActiveEventID = ActiveWWisePBP->Event->GetShortID();

	if (!!!PlayingID)
	{
		// TODO: Add warning
	}
	else
	{
		// TODO: Exeute OnsSart events if there are some
		// TODO: Bind OnFinish ???
	}
}

void UMTWPAudioInstance_WWise::Stop()
{
	if (!!!IsValid())
	{
		// TODO: Add warning
	}
	
	auto ActiveWWisePBP = Cast<UMTWPPlaybackParameters_WWise>(ActivePlaybackParameters);
	FAkAudioDevice::Get()->ExecuteActionOnPlayingID(AkActionOnEventType::Stop, PlayingID,
		(ActiveWWisePBP && ActiveWWisePBP->IsValid()) ? ActiveWWisePBP->FadeOutMsec : 0,
		(ActiveWWisePBP && ActiveWWisePBP->IsValid()) ? ActiveWWisePBP->FadeOutCurveType : EAkCurveInterpolation::Linear);
}

void UMTWPAudioInstance_WWise::SetPlaybackParameters(const UMTWPPlaybackParameters* InPlaybackParameters)
{
	if (!!!IsValid())
	{
		// TODO: Add log
		return;
	}
	
	auto InPBPWWise = Cast<UMTWPPlaybackParameters_WWise>(InPlaybackParameters);
	if (!!!(InPBPWWise && InPBPWWise->IsValid()))
	{
		// TODO: Add log
		return;
	}


	auto ActiveWWisePBP = Cast<UMTWPPlaybackParameters_WWise>(ActivePlaybackParameters);
	if (ActiveWWisePBP && ActiveWWisePBP->IsValid() && ActiveWWisePBP->IsEqual(InPBPWWise))
    {
        return;
    }


	if (ActiveWWisePBP->Event.IsResolved() && ActiveWWisePBP->Event->GetShortID() != ActiveEventID)
	{
		if (PlayingID)
		{
			auto TempPlayingID = AK_INVALID_PLAYING_ID;
			if (Component.IsResolved())
			{
				TempPlayingID = Component->PostAkEvent(InPBPWWise->Event);
			}
			else
			{
				// TODO: Create new CrossWorld 2D component
				// TempPlayingID = ...;
			}

			if (TempPlayingID)
			{
				FAkAudioDevice::Get()->ExecuteActionOnPlayingID(AkActionOnEventType::Stop, PlayingID,
					ActivePlaybackParameters ? ActivePlaybackParameters->FadeOutMsec : 0
					, ActiveWWisePBP ? ActiveWWisePBP->FadeOutCurveType : EAkCurveInterpolation::Linear);
				PlayingID = TempPlayingID;
				ActiveWWisePBP->Event = InPBPWWise->Event;
                ActiveEventID = ActiveWWisePBP->Event->GetShortID();
			}
		}
		// ????
		//ActiveEventID = AK_INVALID_UNIQUE_ID;
		//ActivePlaybackParameters.Event = PlaybackParametersWWise->Event;
	}

		if (InPBPWWise->Switch.IsValid())
		{
			// check if switch are the same
			
			if (ActiveWWisePBP->Switch.IsValid() && ActiveWWisePBP->Switch == InPBPWWise->Switch)
            {
                return;
            }

			if (Component.IsResolved())
			{
				Component->SetSwitch(InPBPWWise->Switch.Switch, InPBPWWise->Switch.SwitchGroup, InPBPWWise->Switch.NameString);
			}

			// TODO: Find the way how to set switch using PlayingID
			// It should be quite tricky because there is no direct method for it
			// It may be an API call but I have not found it yet
			else if (FAkAudioDevice::Get())
			{
				// ...TODO: Creating a new AudioComponent...
			}
			ActiveWWisePBP->Switch = InPBPWWise->Switch;
		}

		if (!InPBPWWise->RtpcSet.IsEmpty())
		{
			if (ActiveWWisePBP->RtpcSet == InPBPWWise->RtpcSet)
            {
                return;
            }
			for (const FMTWPRtpc_WWise& Rtpc : PlaybackParametersWWise->RtpcSet)
			{
				if (Rtpc.IsValid())
				{
					if (Component.IsResolved())
					{
						Component->SetRTPCValue(Rtpc.Rtpc, Rtpc.Value, Rtpc.InterpolationTimeMs, Rtpc.NameString);
					}
					else if (FAkAudioDevice::Get() && Rtpc.Rtpc.IsResolved())
					{
						FAkAudioDevice::Get()->SetRTPCValueByPlayingID(Rtpc.Id, Rtpc.Value, PlayingID, Rtpc.InterpolationTimeMs);
					}

					if (auto FoundRtpc = ActivePlaybackParameters.RtpcSet.Find(Rtpc))
					{
						// Just "refreshing" dead UObject pointer
						if (!!!FoundRtpc->Rtpc.IsResolved())
						{
							FoundRtpc->Rtpc = Rtpc.Rtpc;
						}
						FoundRtpc->Value = Rtpc.Value;
					}
					else
					{
						ActivePlaybackParameters.RtpcSet.Add(Rtpc);
					}
				}
			}
		}
}

bool FMTWPSwitch_Wwise::IsValid() const
{
	if (Switch.IsResolved())
	{
		NameString = Switch.GetName();
		return true;
	}
	return !NameString.IsEmpty();
}

bool FMTWPRtpc_WWise::IsValid() const
{
	if (Rtpc.IsResolved())
	{
		Id = Rtpc->GetWwiseShortID();
		NameString = Rtpc->GetName();
		return true;
	}
	return Id && !NameString.IsEmpty();
}

FMTWPAudioInstance UMTWPAudioInterface_WWise::CreateAudioInstance2D(const FMTWPPlaybackParameters& PlaybackParameters, const bool bCrossWorld)
{
	// TODO: Add playback validation
	//if (!!!IsPlaybackAllowed())
	//{
	//	return UMTWPAudioSystem_WWise::AudioInstanceDefinition();
	//}

	if (!!!PlaybackParameters.IsValid())
	{
		// TODO: Add warning
		return FMTWPAudioInstance_WWise();
	}

	auto CurrentWorld = GWorld.GetReference();
	if (!!!IsValid(CurrentWorld) || !!!IsValid(CurrentWorld->GetWorldSettings()))
	{
		// TODO: Add warning
		return FMTWPAudioInstance_WWise();
	}

	bool bCreated;
	auto ResultAudioComponent = UAkGameplayStatics::GetAkComponent(CurrentWorld->GetWorldSettings()->GetRootComponent(), bCreated);
	FMTWPAudioInstance_WWise AudioInstance;
	AudioInstance.SetPlaybackParameters(PlaybackParameters);

	AudioInstance.Component->
	return AudioInstance;
}

FMTWPAudioInstance UMTWPAudioInterface_WWise::CreateAudioInstanceAtLocation(const FMTWPPlaybackParameters& AudioEntity, const FVector& Location, const FRotator& Orientation)
{
	// TODO
	return FMTWPAudioInstance_WWise();
}


FMTWPAudioInstance UMTWPAudioInterface_WWise::CreateAudioInstanceAttached(const FMTWPPlaybackParameters& AudioEntity, USceneComponent* AttachComponent, FName AttachPointName, const FVector& Location, const FRotator& Orientation, EAttachmentRule AttachmentRule)
{
	// TODO: 
	return FMTWPAudioInstance_WWise();
}
