// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraInputConfig.h"
// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraInputConfig.h"
#include "InputMappingContext.h"
//#include "Settings/LyraSettingsLocal.h"


ULyraInputConfig::ULyraInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* ULyraInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FLyraInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	//if (bLogNotFound)
	//{
	//	UE_LOG(LogLyra, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	//}

	return nullptr;
}

const UInputAction* ULyraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FLyraInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	//if (bLogNotFound)
	//{
	//	UE_LOG(LogLyra, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	//}

	return nullptr;
}