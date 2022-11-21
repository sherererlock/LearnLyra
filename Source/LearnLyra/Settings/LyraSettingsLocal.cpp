// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraSettingsLocal.h"
#include "ICommonUIModule.h"
#include "CommonUISettings.h"
#include "PlayerMappableInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "Player/LyraLocalPlayer.h"

ULyraSettingsLocal::ULyraSettingsLocal()
{

}

ULyraSettingsLocal* ULyraSettingsLocal::Get()
{
	return GEngine ? CastChecked<ULyraSettingsLocal>(GEngine->GetGameUserSettings()) : nullptr;
}

void ULyraSettingsLocal::SetControllerPlatform(const FName InControllerPlatform)
{
	if (ControllerPlatform != InControllerPlatform)
	{
		ControllerPlatform = InControllerPlatform;

		// Apply the change to the common input subsystem so that we refresh any input icons we're using.
		if (UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(GetTypedOuter<ULocalPlayer>()))
		{
			InputSubsystem->SetGamepadInputType(ControllerPlatform);
		}
	}
}

FName ULyraSettingsLocal::GetControllerPlatform() const
{
	return ControllerPlatform;
}

void ULyraSettingsLocal::RegisterInputConfig(ECommonInputType Type, const UPlayerMappableInputConfig* NewConfig, const bool bIsActive)
{
	if (NewConfig)
	{
		const int32 ExistingConfigIdx = RegisteredInputConfigs.IndexOfByPredicate([&NewConfig](const FLoadedMappableConfigPair& Pair) { return Pair.Config == NewConfig; });
		if (ExistingConfigIdx == INDEX_NONE)
		{
			const int32 NumAdded = RegisteredInputConfigs.Add(FLoadedMappableConfigPair(NewConfig, Type, bIsActive));
			if (NumAdded != INDEX_NONE)
			{
				OnInputConfigRegistered.Broadcast(RegisteredInputConfigs[NumAdded]);
			}
		}
	}
}

int32 ULyraSettingsLocal::UnregisterInputConfig(const UPlayerMappableInputConfig* ConfigToRemove)
{
	if (ConfigToRemove)
	{
		const int32 Index = RegisteredInputConfigs.IndexOfByPredicate([&ConfigToRemove](const FLoadedMappableConfigPair& Pair) { return Pair.Config == ConfigToRemove; });
		if (Index != INDEX_NONE)
		{
			RegisteredInputConfigs.RemoveAt(Index);
			return 1;
		}

	}
	return INDEX_NONE;
}

void ULyraSettingsLocal::ActivateInputConfig(const UPlayerMappableInputConfig* Config)
{
	if (Config)
	{
		const int32 ExistingConfigIdx = RegisteredInputConfigs.IndexOfByPredicate([&Config](const FLoadedMappableConfigPair& Pair) { return Pair.Config == Config; });
		if (ExistingConfigIdx != INDEX_NONE)
		{
			RegisteredInputConfigs[ExistingConfigIdx].bIsActive = true;
			OnInputConfigActivated.Broadcast(RegisteredInputConfigs[ExistingConfigIdx]);
		}
	}
}

void ULyraSettingsLocal::DeactivateInputConfig(const UPlayerMappableInputConfig* Config)
{
	if (Config)
	{
		const int32 ExistingConfigIdx = RegisteredInputConfigs.IndexOfByPredicate([&Config](const FLoadedMappableConfigPair& Pair) { return Pair.Config == Config; });
		if (ExistingConfigIdx != INDEX_NONE)
		{
			RegisteredInputConfigs[ExistingConfigIdx].bIsActive = false;
			OnInputConfigDeactivated.Broadcast(RegisteredInputConfigs[ExistingConfigIdx]);
		}
	}
}

const UPlayerMappableInputConfig* ULyraSettingsLocal::GetInputConfigByName(FName ConfigName) const
{
	for (const FLoadedMappableConfigPair& Pair : RegisteredInputConfigs)
	{
		if (Pair.Config->GetConfigName() == ConfigName)
		{
			return Pair.Config;
		}
	}
	return nullptr;
}

void ULyraSettingsLocal::GetRegisteredInputConfigsOfType(ECommonInputType Type, TArray<FLoadedMappableConfigPair>& OutArray) const
{
	OutArray.Empty();

	// If "Count" is passed in then 
	if (Type == ECommonInputType::Count)
	{
		OutArray = RegisteredInputConfigs;
		return;
	}

	for (const FLoadedMappableConfigPair& Pair : RegisteredInputConfigs)
	{
		if (Pair.Type == Type)
		{
			OutArray.Emplace(Pair);
		}
	}
}

void ULyraSettingsLocal::AddOrUpdateCustomKeyboardBindings(const FName MappingName, const FKey NewKey, ULyraLocalPlayer* LocalPlayer)
{
	if (MappingName == NAME_None)
	{
		return;
	}

	if (InputConfigName != TEXT("Custom"))
	{
		// Copy Presets.
		if (const UPlayerMappableInputConfig* DefaultConfig = GetInputConfigByName(TEXT("Default")))
		{
			for (const FEnhancedActionKeyMapping& Mapping : DefaultConfig->GetPlayerMappableKeys())
			{
				// Make sure that the mapping has a valid name, its possible to have an empty name
				// if someone has marked a mapping as "Player Mappabe" but deleted the default field value
				if (Mapping.PlayerMappableOptions.Name != NAME_None)
				{
					CustomKeyboardConfig.Add(Mapping.PlayerMappableOptions.Name, Mapping.Key);
				}
			}
		}

		InputConfigName = TEXT("Custom");
	}

	if (FKey* ExistingMapping = CustomKeyboardConfig.Find(MappingName))
	{
		// Change the key to the new one
		CustomKeyboardConfig[MappingName] = NewKey;
	}
	else
	{
		CustomKeyboardConfig.Add(MappingName, NewKey);
	}

	// Tell the enhanced input subsystem for this local player that we should remap some input! Woo
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->AddPlayerMappedKey(MappingName, NewKey);
	}
}
