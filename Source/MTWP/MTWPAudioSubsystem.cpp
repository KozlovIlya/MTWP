#include "MTWPAudioSubsystem.h"

void UMTWPPlaybackParameterNumeric::SetValue(const float NewValue)
{
	Value = NewValue;
	if (AudioInstance.IsResolved() && AudioInstance->IsValid())
	{
		/*AudioInstance->SetPlaybackParameterNumeric(this, Value);*/
	}
}

void UMTWPPlaybackParameterString::SetValue(const FString& NewValue)
{
	Value = NewValue;
	if (AudioInstance.IsResolved() && AudioInstance->IsValid())
	{
		/*A
		udioInstance->SetPlaybackParameterString(this, Value);*/
	}
}

void UMTWPPlaybackParameterBool::SetValue(const bool NewValue)
{
	Value = NewValue;
	if (AudioInstance.IsResolved() && AudioInstance->IsValid())
	{
		/*AudioInstance->SetPlaybackParameterBool(this, Value);*/
	}
}
