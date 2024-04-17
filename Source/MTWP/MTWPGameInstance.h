#pragma once

#include "CoreMinimal.h"
#include "MTWPAudioSubsystem.h"

#include "MTWPGameInstance.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MTWP_API UMTWPGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, Instanced)
    TObjectPtr<UMTWPAudioEntity> AmbientAudioEntity;

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UMTWPAudioInstance> AmbientAudioInstance;
};