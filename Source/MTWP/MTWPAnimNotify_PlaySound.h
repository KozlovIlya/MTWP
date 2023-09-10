#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify_PlaySound.h"

#include "MTWPAnimNotify_PlaySound.generated.h"


UCLASS(BLueprintType, Blueprintable)
class MTWP_API UMTWPAnimNotify_PlaySound : public UAnimNotify_PlaySound
{

	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UFMODEvent> Event;
};