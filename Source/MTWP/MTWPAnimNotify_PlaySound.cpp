#include "MTWPAnimNotify_PlaySound.h"

#include "FMODEvent.h"
#include "FMODBlueprintStatics.h"

void UMTWPAnimNotify_PlaySound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	if (!IsValid(Event) || !IsValid(MeshComp))
	{
		return;
	}

	//if (!Sound->IsOneShot())
	//{
	//	UE_LOG(LogAudio, Warning, TEXT("PlaySound notify: Anim %s tried to play a sound asset which is not a one-shot: '%s'. Spawning suppressed."), *GetNameSafe(Animation), *GetNameSafe(Sound));
	//	return;
	//}

#if WITH_EDITORONLY_DATA
	UWorld* World = MeshComp->GetWorld();
	if (bPreviewIgnoreAttenuation && World && World->WorldType == EWorldType::EditorPreview)
	{
		UFMODBlueprintStatics::PlayEvent2D(World, Event, true);
	}
	else
#endif
	if (bFollow)
	{
		UFMODBlueprintStatics::PlayEventAttached(Event, MeshComp, AttachName, FVector(ForceInit), EAttachLocation::SnapToTarget, true, true, true);
	}
	else
	{
		UFMODBlueprintStatics::PlayEventAtLocation(MeshComp, Event, MeshComp->GetComponentTransform(), true);
	}	
}
