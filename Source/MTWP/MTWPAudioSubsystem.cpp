#include "MTWPAudioSubsystem.h"



//template<typename T>
//void UMTWPAudioInstance::SetParameter(const FName& Name, T::SetValueType Value)
//{
//    if (auto Parameter = Cast<T>(GetPlaybackParameter(Name)); IsValid(Parameter))
//    {
//        if (Parameter->IsValidValue(Value))
//        {
//            if (Parameter->GetValue() != Value)
//            {
//                Parameter->SetPreviousValue(Parameter->GetValue());
//                Parameter->SetValue(Value);
//                PlaybackParameterChanged.Broadcast(Name);
//            }
//            else
//            {
//                UE_LOG(LogTemp, Warning, TEXT("Value %s is already set for parameter name %s, type %s"), ToString(Value), *Name.ToString(), *ParamType::StaticClass().GetName());
//            }
//        }
//        else
//        {
//            UE_LOG(LogTemp, Warning, TEXT("Value %s is not valid for parameter name %s type %s"), ToString(Value), *Name.ToString(), *ParamType::StaticClass().GetName());
//        }
//    }
//    else
//    {
//        UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s type %s."), ToString(Value), *ParamType::StaticClass()->GetName());
//    }
//}
//
//template<typename T>
//T::GetValueType UMTWPAudioInstance::GetParameter(const FName& Name) const
//{
//    if (auto Parameter = Cast<T>(GetPlaybackParameterConst(Name)))
//    {
//        return Parameter->GetValue();
//    }
//
//    UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s type %s."), *Name.ToString(), *ParamType::StaticClass()->GetName());
//
//    return T::DefaultValue;
//}
//
//template<typename T>
//T::GetValueType UMTWPAudioInstance::GetPreviousParameter(const FName& Name) const
//{
//    if (auto Parameter = Cast<T>(GetPlaybackParameterConst(Name)))
//    {
//        return Parameter->GetPreviousValue();
//    }
//
//    UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s type %s."), *Name.ToString(), *ParamType::StaticClass()->GetName());
//
//    return T::DefaultValue;
//}


void UMTWPAudioInstance::Play()
{
    //if (GetPlayingStatus() == EMTWPPlayingStatus::Init)
    //{
    //    if (StartLambdaDelegate.IsBound())
    //    {
    //        StartLambdaDelegate.Execute();
    //    }
    //    else
    //    {
    //        UE_LOG(LogTemp, Warning, TEXT("Start delegate is not bound."));
    //    }
    //}
}

void UMTWPAudioInstance::SetParameterValueNumeric(const FName& Name, float Value)
{
    auto Parameter = Cast<UMTWPPlaybackParameterNumeric>(GetPlaybackParameter(Name));
    
    if (UNLIKELY(!!!IsValid(Parameter)))
    {
        UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s type Numeri."), *Name.ToString());
        return;
    }

    if (UNLIKELY(!!!Parameter->IsValidValue(Value)))
    {
        UE_LOG(LogTemp, Warning, TEXT("Value %f is not valid for parameter %s"), Value, *Name.ToString());
        return;
    }
    
    if (Parameter->Value != Value)
    {
        auto TempPreviousValue = Parameter->PreviousValue;
        Parameter->PreviousValue = Parameter->Value;
        Parameter->Value = Value;

        if (LIKELY(UpdateParameterNumeric(Parameter)))
        {
            PlaybackParameterChanged.Broadcast(Name);
        }
        else
        {
            Parameter->Value = Parameter->PreviousValue;
            Parameter->PreviousValue = TempPreviousValue;
            UE_LOG(LogTemp, Warning, TEXT("Failed to update parameter %s with value %f"), *Name.ToString(), Value);
        }
    }
}

void UMTWPAudioInstance::SetParameterValueString(const FName& Name, const FString& Value)
{
    auto Parameter = Cast<UMTWPPlaybackParameterString>(GetPlaybackParameter(Name));
    
    if (UNLIKELY(!!!IsValid(Parameter)))
    {
        UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s type String."), *Name.ToString());
        return;
    }
    
    if (!!!Parameter->IsValidValue(Value))
    {
        UE_LOG(LogTemp, Warning, TEXT("Value %s is not valid for parameter %s"), *Value, *Name.ToString());
        return;
    }
    
    if (Parameter->Value != Value)
    {
        auto TempPreviousValue = Parameter->PreviousValue;
        Parameter->PreviousValue = Parameter->Value;
        Parameter->Value = Value;

        if (LIKELY(UpdateParameterString(Parameter)))
        {
            PlaybackParameterChanged.Broadcast(Name);
        }
        else
        {
            Parameter->Value = Parameter->PreviousValue;
            Parameter->PreviousValue = TempPreviousValue;
            UE_LOG(LogTemp, Warning, TEXT("Failed to update parameter %s with value %s"), *Name.ToString(), *Value);
        }
    }
}

void UMTWPAudioInstance::SetParameterValueBoolean(FName Name, bool Value)
{
    auto Parameter = Cast<UMTWPPlaybackParameterBoolean>(GetPlaybackParameter(Name));
    
    if (!!!IsValid(Parameter))
    {
        UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s type Boolean."), *Name.ToString());
        return;
    }
    
    if (Parameter->Value != Value)
    {
        auto TempPreviousValue = Parameter->PreviousValue;
        Parameter->PreviousValue = Parameter->Value;
        Parameter->Value = Value;

        if (LIKELY(UpdateParameterBoolean(Parameter)))
        {
            Parameter->PreviousValue = Parameter->Value;
            Parameter->Value = Value;
            PlaybackParameterChanged.Broadcast(Name);
        }
        else
        {
            Parameter->Value = Parameter->PreviousValue;
            Parameter->PreviousValue = TempPreviousValue;
            UE_LOG(LogTemp, Warning, TEXT("Failed to update parameter %s with value %d"), *Name.ToString(), Value);
        }
    }
}

void UMTWPAudioInstance::SetParameterValueObjectPointer(const FName& InName, UObject* Value)
{
    auto Parameter = Cast<UMTWPPlaybackParameterObjectPointer>(GetPlaybackParameter(InName));

    if (UNLIKELY(!!!IsValid(Parameter)))
    {
        UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s type ObjectPointer."), *InName.ToString());
        return;
    }
    
    if (UNLIKELY(!!!Parameter->IsValidValue(Value)))
    {
        if (Parameter->GetValue() != Value)
        {
            auto TempPreviousValue = Parameter->GetPreviousValue();
            Parameter->SetPreviousValue(Parameter->GetValue());
            Parameter->SetValue(Value);

            if (LIKELY(UpdateParameterObjectPointer(Parameter)))
            {
                PlaybackParameterChanged.Broadcast(InName);
            }
            else
            {
                Parameter->SetValue(Parameter->GetPreviousValue());
                Parameter->SetPreviousValue(TempPreviousValue);
                UE_LOG(LogTemp, Warning, TEXT("Failed to update parameter %s with value %s"), *InName.ToString(), *Value->GetName());
            }
        }
    }
}



float UMTWPAudioInstance::GetParameterValueNumeric(const FName& Name) const
{
    if (auto Parameter = Cast<UMTWPPlaybackParameterNumeric>(GetPlaybackParameterConst(Name)))
    {
        return Parameter->Value;
    }

    UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s type Numeric."), *Name.ToString());

    return 0.f;
}

FString UMTWPAudioInstance::GetParameterValueString(const FName& Name) const
{
    if (auto Parameter = Cast<UMTWPPlaybackParameterString>(GetPlaybackParameterConst(Name)))
    {
        return Parameter->Value;
    }

    UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s type String."), *Name.ToString());

    return FString();
}

bool UMTWPAudioInstance::GetParameterValueBoolean(const FName& Name) const
{
    if (auto Parameter = Cast<UMTWPPlaybackParameterBoolean>(GetPlaybackParameterConst(Name)))
    {
        return Parameter->Value;
    }

    UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s type Boolean."), *Name.ToString());

    return false;
}

UObject* UMTWPAudioInstance::GetParameterValueObjectPointer(const FName& Name) const
{
    if (auto Parameter = Cast<UMTWPPlaybackParameterObjectPointer>(GetPlaybackParameterConst(Name)))
    {
        return Parameter->GetValue();
    }

    UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s type ObjectPointer."), *Name.ToString());

    return nullptr;
}



float UMTWPAudioInstance::GetPreviousParameterValueNumeric(const FName& Name) const
{
    if (auto Parameter = Cast<UMTWPPlaybackParameterNumeric>(GetPlaybackParameterConst(Name)))
    {
        return Parameter->PreviousValue;
    }

    UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s type Numeri."), *Name.ToString());

    return 0.f;
}

FString UMTWPAudioInstance::GetPreviousParameterValueString(const FName& Name) const
{
    if (auto Parameter = Cast<UMTWPPlaybackParameterString>(GetPlaybackParameterConst(Name)))
    {
        return Parameter->PreviousValue;
    }

    UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s type String."), *Name.ToString());

    return FString();
}

bool UMTWPAudioInstance::GetPreviousParameterValueBoolean(const FName& Name) const
{
    if (auto Parameter = Cast<UMTWPPlaybackParameterBoolean>(GetPlaybackParameterConst(Name)))
    {
        return Parameter->PreviousValue;
    }

    UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s type Boolean."), *Name.ToString());

    return false;
}

 UObject* UMTWPAudioInstance::GetPreviousParameterValueObjectPointer(const FName& Name) const
{
    if (auto Parameter = Cast<UMTWPPlaybackParameterObjectPointer>(GetPlaybackParameterConst(Name)))
    {
        return Parameter->GetPreviousValue();
    }
    
    UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s type ObjectPointer."), *Name.ToString());

    return nullptr;
}


 inline bool UMTWPAudioInstance::UpdateParameter(const FName& Name)
 {
     if (auto Parameter = GetPlaybackParameter(Name))
     {
         switch (Parameter->GetType())
         {
         case EMTWPPlaybackParameterType::Numeric:
             return UpdateParameterNumeric(Cast<UMTWPPlaybackParameterNumeric>(Parameter));
         case EMTWPPlaybackParameterType::String:
             return UpdateParameterString(Cast<UMTWPPlaybackParameterString>(Parameter));
         case EMTWPPlaybackParameterType::Boolean:
             return UpdateParameterBoolean(Cast<UMTWPPlaybackParameterBoolean>(Parameter));
         case EMTWPPlaybackParameterType::ObjectPointer:
             return UpdateParameterObjectPointer(Cast<UMTWPPlaybackParameterObjectPointer>(Parameter));
         default:
             UE_LOG(LogTemp, Warning, TEXT("Parameter type %s is not supported."));
             return false;
         }
     }
     else
     {
         UE_LOG(LogTemp, Warning, TEXT("No parameter with the name %s."), *Name.ToString());
         return false;
     }
 }

 bool UMTWPAudioInstance::AddPlaybackParameter(UMTWPPlaybackParameter* InPlaybackParameter)
 {
     if (InPlaybackParameter)
     {
         if (PlaybackParametersIndexMap.Contains(InPlaybackParameter->GetName()))
         {
             UE_LOG(LogTemp, Warning, TEXT("Playback parameter with the name %s already exists."), *InPlaybackParameter->GetName().ToString());
             return false;
         }
         else
         {
             return PlaybackParametersIndexMap.Add(InPlaybackParameter->GetName(), PlaybackParameters.Add(InPlaybackParameter)) > 0;
         }
     }
     else
     {
         UE_LOG(LogTemp, Warning, TEXT("Playback parameter is nullptr."));
         return false;
     }
 }


 UMTWPPlaybackParameter* UMTWPAudioInstance::GetPlaybackParameter(const FName& Name)
 {
     if (auto Index = PlaybackParametersIndexMap.Find(Name); Index && PlaybackParameters.IsValidIndex(*Index))
     {
         return PlaybackParameters[*Index];
     }
     else
     {
         UE_LOG(LogTemp, Warning, TEXT( "No playback parameter with the name %s."), *Name.ToString());
         return nullptr;
     }
 }

 UMTWPAudioInstance* UMTWPAudioSubsystem::CreateAudioInstance2D(UMTWPAudioEntity* InEntity, bool bInPersistent)
 {
     if (UNLIKELY(!!!IsValid(InEntity)))
     {
         UE_LOG(LogTemp, Warning, TEXT("No entity found."));
         return nullptr;
     };

     auto AudioInterface = GetAudioInterface(InEntity);

     if (UNLIKELY(!!!AudioInterface))
     {
         UE_LOG(LogTemp, Warning, TEXT("No suitable audio interface found for entity %s."), *InEntity->GetName());
         return nullptr;
     }

     return AudioInterface->CreateAudioInstance2D(InEntity, bInPersistent);
}

 UMTWPAudioInstance* UMTWPAudioSubsystem::CreateAudioInstanceAtLocation(UMTWPAudioEntity* InEntity, const FVector& Location, const FRotator& Orientation)
 {
     if (UNLIKELY(!!!IsValid(InEntity)))
     {
         UE_LOG(LogTemp, Warning, TEXT("No entity found."));
         return nullptr;
     };

     auto AudioInterface = GetAudioInterface(InEntity);

     if (UNLIKELY(!!!AudioInterface))
     {
         UE_LOG(LogTemp, Warning, TEXT("No suitable audio interface found for entity %s."), *InEntity->GetName());
         return nullptr;
     }

     return AudioInterface->CreateAudioInstanceAtLocation(InEntity, Location, Orientation);

}

 UMTWPAudioInstance* UMTWPAudioSubsystem::CreateAudioInstanceAttached(UMTWPAudioEntity* InEntity, USceneComponent* AttachComponent, FName AttachPointName, const FVector& Location, const FRotator& Orientation, const EAttachmentRule& InAttachmentRule)
 {
     if (UNLIKELY(!!!IsValid(InEntity)))
     {
         UE_LOG(LogTemp, Warning, TEXT("No entity found."));
         return nullptr;
     };

     auto AudioInterface = GetAudioInterface(InEntity);

     if (UNLIKELY(!!!AudioInterface))
     {
         UE_LOG(LogTemp, Warning, TEXT("No suitable audio interface found for entity %s."), *InEntity->GetName());
         return nullptr;
     }
     
     return AudioInterface->CreateAudioInstanceAttached(InEntity, AttachComponent, AttachPointName, Location, Orientation, InAttachmentRule);
 }

 UMTWPAudioInterface* UMTWPAudioEntity::CreateAudioInterface() const
 {
     return NewObject<UMTWPAudioInterface>();
 }

 UMTWPAudioInterface* UMTWPAudioSubsystem::GetAudioInterface(const UMTWPAudioEntity* Entity)
 {
     if (UNLIKELY(!!!IsValid(Entity->StaticClass())))
     {
         UE_LOG(LogTemp, Warning, TEXT("Entity class is not valid."));
         return nullptr;
     }

     auto AudioEntityClassName = FName(Entity->StaticClass()->GetName());
     auto AudioInterfaceIdxPtr = AudioInterfaceIndexMap.Find(FName(AudioEntityClassName));
     int AudioInterfaceIdx = AudioInterfaceIdxPtr ? *AudioInterfaceIdxPtr : -1;
     if (!!!AudioInterfaceIdxPtr)
     {
         AudioInterfaceIdx = AudioInterfaces.Add(Entity->CreateAudioInterface());
         AudioInterfaceIndexMap.Add(FName(AudioEntityClassName), AudioInterfaceIdx);
     }

     if (UNLIKELY(AudioInterfaceIdx < 0))
     {
         UE_LOG(LogTemp, Warning, TEXT("Failed to get audio interface."));
         return nullptr;
     }

     return AudioInterfaces[AudioInterfaceIdx];
 }
