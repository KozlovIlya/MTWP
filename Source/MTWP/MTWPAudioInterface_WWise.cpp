#include "MTWPAudioInterface_WWise.h"

#include <Wwise/API/WwiseSoundEngineAPI.h>

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
	if (UNLIKELY(GetPlayingStatus() == EMTWPPlayingStatus::Playing || 
	GetPlayingStatus() == EMTWPPlayingStatus::Destroying))
    {
        return;
    }
	else if (GetPlayingStatus() == EMTWPPlayingStatus::Paused)
	{
		FAkAudioDevice::Get()->ExecuteActionOnPlayingID(AkActionOnEventType::Resume, PlayingID, TransitionDurationMs, CurveInterpolation);
	}
	else if (GetPlayingStatus() == EMTWPPlayingStatus::Init)
	{
		if (UNLIKELY(!!!IsValid(Event)))
		{
			UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInstance_WWise::Play: Event is not valid"));
			return;
		}

		if (!!!IsValid(Component))
		{
			if (PlayingType != EMTWPPlayingType::Flat)
			{
				UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInstance_WWise::Play: Component is not valid"));
				return;
			}
			Component = CreateAkComponent2D();
			for (auto Parameter : PlaybackParameters)
            {
                UpdateParameter(Parameter->GetName());
            }
			PlayingID = FAkAudioDevice::Get()->PostAkAudioEventOnComponent(Event, Component, 0U, nullptr, nullptr, true);
		}
		else
		{
			PlayingID = Component->PostAkEvent(Event);
		}

		if (UNLIKELY(!!!PlayingID))
        {
            UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInstance_WWise::Play: PlayingID is not valid"));
            return;
        }
		PlayingStatus = EMTWPPlayingStatus::Playing;
	}
}

void UMTWPAudioInstance_WWise::Stop()
{
	if (UNLIKELY(!!!FAkAudioDevice::Get()))
	{
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInstance_WWise::Stop: FAkAudioDevice is not valid"));
	}

	FAkAudioDevice::Get()->ExecuteActionOnPlayingID(AkActionOnEventType::Stop, PlayingID, TransitionDurationMs, CurveInterpolation);
	
	if (!!!IsValid(Component) && PlayingType != EMTWPPlayingType::Flat && !!!bPersistent)
	{
		PlayingStatus = EMTWPPlayingStatus::Destroying;
		MarkAsGarbage(); // Should we really need that?
	}
	else
	{
		PlayingStatus = EMTWPPlayingStatus::Init;
	}
}

bool UMTWPAudioInstance_WWise::UpdateParameterNumeric(UMTWPPlaybackParameterNumeric* Parameter)
{
	auto RTPCParam = Cast<UMTWParameterPRTPC_WWise>(Parameter);
	if (!!!IsValid(RTPCParam) || !!!IsValid(RTPCParam->RtpcObject) || !!!FAkAudioDevice::Get())
	{
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInstance_WWise::UpdateRTPC: RTPCParam or RTPCObject is not valid"));
		return false;
	}

	AKRESULT Result = AK_Fail;

	if (IsValid(Component))
    {
		if (auto* SoundEngine = IWwiseSoundEngineAPI::Get(); LIKELY(SoundEngine))
		{
			Result = SoundEngine->SetRTPCValue(RTPCParam->RtpcObject->GetShortID(), RTPCParam->Value, Component->GetAkGameObjectID(), RTPCParam->InterpolationTimeMs);
		}
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInstance_WWise::UpdateRTPC: SoundEngine is not valid"));
        }
	}
	else
	{
		Result = FAkAudioDevice::Get()->SetRTPCValueByPlayingID(RTPCParam->RtpcObject->GetShortID(), RTPCParam->Value, PlayingID, RTPCParam->InterpolationTimeMs);
	}

	if (Result != AK_Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInstance_WWise::UpdateRTPC: RTPC could not be set for sound 2D"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("MTWPAudioInstance_WWise::UpdateRTPC: RTPC set for sound 2D"));
	}

	return Result == AK_Success;

}

bool UMTWPAudioInstance_WWise::UpdateParameterString(UMTWPPlaybackParameterString* InParameter)
{	
	if (UNLIKELY(!!!IsValid(Component)))
	{
		auto bWasPlaying = FAkAudioDevice::Get()->IsPlayingIDActive(Event->GetShortID(), PlayingID);

		if (UNLIKELY(!!!FAkAudioDevice::Get()))
		{
			UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInstance_WWise::UpdateSwitch: FAkAudioDevice is not valid"));
			return false;
		}
		
		// TODO: Learn more about spatial sound in WWise
		// It's deprecated, but may be useful
		//FAkAudioDevice::GetSpatialAudioListener();

		if (bWasPlaying)
		{
			FAkAudioDevice::Get()->ExecuteActionOnPlayingID(AkActionOnEventType::Stop, PlayingID, TransitionDurationMs, CurveInterpolation);
			PlayingID = AK_INVALID_PLAYING_ID;
			PlayingStatus = EMTWPPlayingStatus::Init;
		}

		auto ListnerComponent = *FAkAudioDevice::Get()->GetDefaultListeners().begin();
		Component = NewObject<UMTWPAudioComponent_WWise>();
		if (UNLIKELY(!!!IsValid(Component) || !!!IsValid(ListnerComponent)))
		{
			UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::UpdateParameterStrig: AudioComponent is not valid"));
			return false;
		}

		Component->RegisterComponentWithWorld(ListnerComponent->GetWorld());
		Component->AttachToComponent(ListnerComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Component->OcclusionCollisionChannel = TEnumAsByte(EAkCollisionChannel::EAKCC_Camera);
		Component->bPersistent = true; // only persistent sounds can be alive on this stage
		Component->SetAutoDestroy(false);

		for (auto Parameter : PlaybackParameters)
		{
			UpdateParameter(Parameter->GetName());
        }

		if (bWasPlaying)
		{
			Play();
		}
	}
	else
	{
		// TODO: Add validation
		Component->SetSwitch(nullptr, InParameter->GetName().ToString(), InParameter->GetValue());
	}
	return true;
}

void UMTWPAudioInstance_WWise::SetupParams(const UMTWPAudioEntity_WWise& InEntityChecked)
{
	for (auto RTPC : InEntityChecked.RTPCs)
	{
		if (UNLIKELY(!!!RTPC || !!!RTPC->RtpcObject))
		{
			UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: RTPC is not valid"));
			continue;
		}

		//auto Result = FAkAudioDevice::Get()->SetRTPCValueByPlayingID(RTPC->RtpcObject->GetShortID(), RTPC->Value, AudioInstance->PlayingID, RTPC->InterpolationTimeMs);
		/*auto Result = */Component->SetRTPCValue(RTPC->RtpcObject, RTPC->Value, RTPC->InterpolationTimeMs, "");
		auto Result = AK_Success;

		if (UNLIKELY(Result != AK_Success))
		{
			UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: RTPC could not be set for sound 2D"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: RTPC set for sound 2D"));
		}
		AddPlaybackParameter(RTPC);
	}

	for (auto i = 0; i < InEntityChecked.Switches.Num(); ++i)
	{
		auto Switch = InEntityChecked.Switches[i];
		if (UNLIKELY(!!!Switch || !!!Switch->IsValidValue(Switch->GetValue())))
		{
			UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: Switch is not valid"));
			continue;
		}
		AddPlaybackParameter(Switch);
	}

	OnWorldCleanupHandle = FWorldDelegates::OnWorldCleanup.AddLambda([Instance = this](UWorld* World, bool bSessionEnded, bool bCleanupResources)
		{
			if (IsValid(Instance))
			{
				if (bCleanupResources)
				{
					if (!!!Instance->bPersistent)
					{
						Instance->Stop();
					}
					Instance->Component = nullptr;
				}
				
			}
		});
}

UMTWPAudioComponent_WWise* UMTWPAudioInstance_WWise::CreateAkComponent2D() const
{
	// TODO: Need to contain 2D-sound listner somewhere.
	//AudioInstance->Component = NewObject<UMTWPAudioComponent_WWise>(*FAkAudioDevice::Get()->GetDefaultListeners().begin());

	if (UNLIKELY(!!!FAkAudioDevice::Get()->GetDefaultListeners().begin()))
	{
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: AudioComponent is not valid"));
		return nullptr;
	}

	auto ListnerComponent = *FAkAudioDevice::Get()->GetDefaultListeners().begin();
	auto NewComponent = NewObject<UMTWPAudioComponent_WWise>();
	if (UNLIKELY(!!!IsValid(NewComponent) || !!!IsValid(ListnerComponent)))
	{
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: AudioComponent is not valid"));
		return nullptr;
	}

	NewComponent->RegisterComponentWithWorld(ListnerComponent->GetWorld());
	NewComponent->AttachToComponent(ListnerComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	NewComponent->OcclusionCollisionChannel = TEnumAsByte(EAkCollisionChannel::EAKCC_Camera);
	NewComponent->SetAutoDestroy(false);

	NewComponent->bPersistent = bPersistent;

	return NewComponent;
}

void UMTWPAudioInstance_WWise::BeginDestroy()
{
	FWorldDelegates::OnWorldCleanup.Remove(OnWorldCleanupHandle);
	Super::BeginDestroy();
}


UMTWPAudioInstance* UMTWPAudioInterface_WWise::CreateAudioInstance2D(UMTWPAudioEntity* InEntity, bool bInPersistent)
{
	// TODO: Add playback validation
	//if (!!!IsPlaybackAllowed())
	//{
	//	return UMTWPAudioSystem_WWise::AudioInstanceDefinition();
	//}


	if (UNLIKELY(!!!GetWorld() || !!!FAkAudioDevice::Get()))
	{
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: World or FAkAudioDevice is not valid"));
		return nullptr;
	}

	auto InEntity_WWise = Cast<UMTWPAudioEntity_WWise>(InEntity);
	if (UNLIKELY(!!!IsValid(InEntity_WWise)))
	{
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: InEntity_WWise is not valid"));
		return nullptr;
	}

	auto InEventObject = InEntity_WWise->Event;
	if (UNLIKELY(!!!IsValid(InEventObject)))
	{
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: InEventObject is not valid"));
        return nullptr;
    }

	auto AudioInstance = NewObject<UMTWPAudioInstance_WWise>();
	AudioInstance->Event = InEventObject;

	if (UNLIKELY(!!!IsValid(AudioInstance)))
    {
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: AudioInstance is not valid"));
        return nullptr;
    }

	AudioInstance->bPersistent = bInPersistent;

	AudioInstance->Component = AudioInstance->CreateAkComponent2D();
	AudioInstance->SetupParams(*InEntity_WWise);
	
	return AudioInstance;
}

UMTWPAudioInstance* UMTWPAudioInterface_WWise::CreateAudioInstanceAtLocation(UMTWPAudioEntity* InEntity, const FVector& Location, const FRotator& Orientation)
{
	if (UNLIKELY(!!!FAkAudioDevice::Get()))
	{
        UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstanceAtLocation: FAkAudioDevice is not valid"));
        return nullptr;
	}

	if (UNLIKELY(!!!GetWorld()))
    {
        UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstanceAtLocation: World is not valid"));
        return nullptr;
    }

	if (UNLIKELY(!!!IsValid(*FAkAudioDevice::Get()->GetDefaultListeners().begin())))
	{
        UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstanceAtLocation: AudioComponent is not valid"));
        return nullptr;
    }
	
	auto AudioInstance = Cast<UMTWPAudioInstance_WWise>(CreateAudioInstanceAttached(InEntity, *FAkAudioDevice::Get()->GetDefaultListeners().begin(), NAME_None, Location, Orientation, EAttachmentRule::KeepWorld));
	if (!!!IsValid(AudioInstance))
    {
        UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstanceAtLocation: AudioInstance is not valid"));
        return nullptr;
    }
	
	return AudioInstance;
}

UMTWPAudioInstance* UMTWPAudioInterface_WWise::CreateAudioInstanceAttached(UMTWPAudioEntity* InEntity, USceneComponent* AttachComponent, FName AttachPointName, const FVector& Location, const FRotator& Orientation, const EAttachmentRule& InAttachmentRule)
{
	// TODO: Add playback validation
	//if (!!!IsPlaybackAllowed())
	//{
	//	return UMTWPAudioSystem_WWise::AudioInstanceDefinition();
	//}


	if (UNLIKELY(!!!GetWorld() || !!!FAkAudioDevice::Get()))
	{
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: World or FAkAudioDevice is not valid"));
		return nullptr;
	}

	auto InEntity_WWise = Cast<UMTWPAudioEntity_WWise>(InEntity);
	if (UNLIKELY(!!!IsValid(InEntity_WWise)))
	{
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: InEntity_WWise is not valid"));
		return nullptr;
	}

	auto InEventObject = InEntity_WWise->Event;
	if (UNLIKELY(!!!IsValid(InEventObject)))
	{
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: InEventObject is not valid"));
		return nullptr;
	}
	
	if (UNLIKELY(!!!IsValid(AttachComponent)))
	{
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: AttachComponent is not valid"));
		return nullptr;
	}

	auto AudioInstance = NewObject<UMTWPAudioInstance_WWise>();

	if (UNLIKELY(!!!IsValid(AudioInstance)))
	{
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: AudioInstance is not valid"));
		return nullptr;
	}

	AudioInstance->Event = InEventObject;

	AudioInstance->Component = NewObject<UMTWPAudioComponent_WWise>(AudioInstance);
	if (UNLIKELY(!!!IsValid(AudioInstance->Component)))
	{
		UE_LOG(LogTemp, Warning, TEXT("MTWPAudioInterface_WWise::CreateAudioInstance2D: AudioComponent is not valid"));
		return nullptr;
	}

	AudioInstance->Component->RegisterComponentWithWorld(GetWorld());
	auto& ComponentTransform = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	if (InAttachmentRule == EAttachmentRule::KeepRelative)
    {
        ComponentTransform = FAttachmentTransformRules::KeepRelativeTransform;
    }
	else if (InAttachmentRule == EAttachmentRule::KeepWorld)
	{
		ComponentTransform = FAttachmentTransformRules::KeepWorldTransform;
	}
	AudioInstance->Component->AttachToComponent(AttachComponent, ComponentTransform, AttachPointName);
	AudioInstance->Component->SetWorldLocationAndRotation(Location, Orientation);
	
	if (InAttachmentRule == EAttachmentRule::KeepWorld)
	{
		AudioInstance->Component->SetWorldLocationAndRotation(Location, Orientation);
	}
	else
	{
		AudioInstance->Component->SetRelativeLocationAndRotation(Location, Orientation);
	}

	// TODO:
	// Reverb and everything setup
	// Look into AkAudioEvent::PostAkEventAtLocation
	//AkSoundPosition SoundPosition;
	//FQuat OrientationQuat(Orientation);
	//FAkAudioDevice::Get()->FVectorsToAKWorldTransform(Location, OrientationQuat.GetForwardVector(), OrientationQuat.GetUpVector(), SoundPosition);
	//Result = FAkAudioDevice::Get()->SetPosition(AudioInstance->Component, SoundPosition);

	AudioInstance->Component->SetAutoDestroy(false);
	AudioInstance->Component->OcclusionCollisionChannel = TEnumAsByte(EAkCollisionChannel::EAKCC_WorldDynamic);

	AudioInstance->SetupParams(*InEntity_WWise);

	return AudioInstance;
}

bool UMTWPAudioInterface_WWise::IsValidEntity(UMTWPAudioEntity* InEntity) const
{
	return false;
}
