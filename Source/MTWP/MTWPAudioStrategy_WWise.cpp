#include "MTWPAudioStrategy_WWise.h"

#include "AkGameplayStatics.h"

#include <AkComponent.h>
#include <AkAudioEvent.h>
#include <AkSwitchValue.h>
#include <AkRtpc.h>

bool FMTWPAudioInstanceDefinition_WWise::IsValid() const
{
	return Component.IsResolved() || (FAkAudioDevice::Get() && FAkAudioDevice::Get()->IsPlayingIDActive(EventID, PlayingID));
}

bool FMTWPAudioEventDefinition_WWise::IsValid() const
{
	return Event.IsResolved();
}

bool FMTWPSwitchDefinition_Wwise::IsValid() const
{
	// TODO: check what is Switch->GetFullGroupName()
	return Switch.IsResolved() && !SwitchGroup.IsEmpty();
}

bool FMTWPRtpcDefinition_WWise::IsValid() const
{
	return Rtpc.IsResolved();
}

bool FMTWPAudioPlaybackParamsDefinition_WWise::IsValid() const
{
	return SwitchDefinition.IsValid() && RtpcDefinition.IsValid();
}

bool UMTWPAudioSystem_WWise::IsPlaybackAllowed() const
{
	return FAkAudioDevice::Get() && GetWorld() && GetWorld()->AllowAudioPlayback();
}

UMTWPAudioSystem_WWise::AudioInstanceDefinition UMTWPAudioSystem_WWise::CreateAudioInstance2D(const UMTWPAudioSystem_WWise::AudioEventDefinition& InEventDifinition, const UMTWPAudioSystem_WWise::AudioCreationParamsDefinition& InAudioCreationParamsDefinition, const UMTWPAudioSystem_WWise::AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition)
{
	if (!!!IsPlaybackAllowed())
	{
		return UMTWPAudioSystem_WWise::AudioInstanceDefinition();
	}
	if (!!!GetWorld() || !!!IsValid(GetWorld()->GetFirstPlayerController()))
	{
		return UMTWPAudioSystem_WWise::AudioInstanceDefinition();
	}
	bool bCreated;
	auto ResultAudioComponent = UAkGameplayStatics::GetAkComponent(GetWorld()->GetFirstPlayerController()->GetRootComponent(), bCreated);
	UMTWPAudioSystem_WWise::AudioInstanceDefinition AudioInstanceDefinition;
	AudioInstanceDefinition.Component = ResultAudioComponent;

	return AudioInstanceDefinition;
}

UMTWPAudioSystem_WWise::AudioInstanceDefinition UMTWPAudioSystem_WWise::CreateAudioInstanceAttached(const UMTWPAudioSystem_WWise::AudioEventDefinition& InEventDifinition, const UMTWPAudioSystem_WWise::AudioCreationParamsDefinition& InAudioCreationParamsDefinition, const UMTWPAudioSystem_WWise::AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition)
{
	if (!!!IsPlaybackAllowed())
	{
		return UMTWPAudioSystem_WWise::AudioInstanceDefinition();
	}

	bool bCreated;
	auto ResultAudioComponent = UAkGameplayStatics::GetAkComponent(InAudioCreationParamsDefinition.SceneComponent, bCreated, InAudioCreationParamsDefinition.AttachPointName, InAudioCreationParamsDefinition.Location, InAudioCreationParamsDefinition.AttachLocation);
	if (IsValid(ResultAudioComponent) && InAudioCreationParamsDefinition.AttachLocation == EAttachLocation::KeepWorldPosition)
	{
		ResultAudioComponent->SetWorldRotation(InAudioCreationParamsDefinition.Rotation);
	}
	UMTWPAudioSystem_WWise::AudioInstanceDefinition AudioInstanceDefinition;
	AudioInstanceDefinition.Component = ResultAudioComponent;

	return AudioInstanceDefinition;
}

UMTWPAudioSystem_WWise::AudioInstanceDefinition UMTWPAudioSystem_WWise::CreateAudioInstanceAtLocation(const UMTWPAudioSystem_WWise::AudioEventDefinition& InEventDifinition, const UMTWPAudioSystem_WWise::AudioCreationParamsDefinition& InAudioCreationParamsDefinition, const UMTWPAudioSystem_WWise::AudioPlaybackParamsDefinition& InAudioPlaybackParamsDefinition)
{
	if (!!!IsPlaybackAllowed())
	{
		return UMTWPAudioSystem_WWise::AudioInstanceDefinition();
	}

	bool bCreated = false;
	auto ResultAudioComponent = UAkGameplayStatics::SpawnAkComponentAtLocation(this, InEventDifinition.Event, InAudioCreationParamsDefinition.Location, InAudioCreationParamsDefinition.Rotation, false, FString());
	UMTWPAudioSystem_WWise::AudioInstanceDefinition AudioInstanceDefinition;
	AudioInstanceDefinition.Component = ResultAudioComponent;

	return AudioInstanceDefinition;
}

void UMTWPAudioSystem_WWise::SetPlaybackParams(UMTWPAudioSystem_WWise::AudioInstanceDefinition InAudioInstanceDefinition, const UMTWPAudioSystem_WWise::AudioPlaybackParamsDefinition InAudioPlaybackParamsDifinition)
{
	if (!!!InAudioInstanceDefinition.IsValid() || !!!InAudioPlaybackParamsDifinition.IsValid())
	{
		return;
	}

	if (InAudioPlaybackParamsDifinition.RtpcDefinition.IsValid())
	{
		if (IsValid(InAudioInstanceDefinition.Component))
		{
			InAudioInstanceDefinition.Component->SetRTPCValue(InAudioPlaybackParamsDifinition.RtpcDefinition.Rtpc, InAudioPlaybackParamsDifinition.RtpcDefinition.Value, InAudioPlaybackParamsDifinition.RtpcDefinition.InterpolationTimeMs, "");
		}
		else if (FAkAudioDevice::Get())
		{
			FAkAudioDevice::Get()->SetRTPCValueByPlayingID(InAudioPlaybackParamsDifinition.RtpcDefinition.Rtpc->GetShortID(), InAudioPlaybackParamsDifinition.RtpcDefinition.Value, InAudioInstanceDefinition.PlayingID, InAudioPlaybackParamsDifinition.RtpcDefinition.InterpolationTimeMs);
		}
	}

	if (InAudioPlaybackParamsDifinition.SwitchDefinition.IsValid())
	{
		if (IsValid(InAudioInstanceDefinition.Component))
		{
			InAudioInstanceDefinition.Component->SetSwitch(InAudioPlaybackParamsDifinition.SwitchDefinition.Switch, InAudioPlaybackParamsDifinition.SwitchDefinition.SwitchGroup, "");
		}

		// TODO: Find the way how to set switch using PlayingID
		// It should be quite tricky because there is no direct method for it
		// It may be an API call but I have not found it yet
		else if (FAkAudioDevice::Get())
		{
			// ...
		}
	}
}
