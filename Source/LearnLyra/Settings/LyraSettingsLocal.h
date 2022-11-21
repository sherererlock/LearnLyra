// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Input/LyraMappableConfigPair.h"

#include "LyraSettingsLocal.generated.h"

class UPlayerMappableInputConfig;
class ULyraLocalPlayer;
/**
 * 
 */
UCLASS()
class LEARNLYRA_API ULyraSettingsLocal : public UGameUserSettings
{
	GENERATED_BODY()
	
public:

	ULyraSettingsLocal();

	static ULyraSettingsLocal* Get();

	// Keybindings
public:


	// Sets the controller representation to use, a single platform might support multiple kinds of controllers.  For
	// example, Win64 games could be played with both an XBox or Playstation controller.
	UFUNCTION()
	void SetControllerPlatform(const FName InControllerPlatform);
	UFUNCTION()
	FName GetControllerPlatform() const;

	DECLARE_EVENT_OneParam(ULyraSettingsLocal, FInputConfigDelegate, const FLoadedMappableConfigPair& /*Config*/);

	/** Delegate called when a new input config has been registered */
	FInputConfigDelegate OnInputConfigRegistered;

	/** Delegate called when a registered input config has been activated */
	FInputConfigDelegate OnInputConfigActivated;

	/** Delegate called when a registered input config has been deactivate */
	FInputConfigDelegate OnInputConfigDeactivated;

	/** Register the given input config with the settings to make it available to the player. */
	void RegisterInputConfig(ECommonInputType Type, const UPlayerMappableInputConfig* NewConfig, const bool bIsActive);

	/** Unregister the given input config. Returns the number of configs removed. */
	int32 UnregisterInputConfig(const UPlayerMappableInputConfig* ConfigToRemove);

	/** Set a registered input config as active */
	void ActivateInputConfig(const UPlayerMappableInputConfig* Config);

	/** Deactivate a registered config */
	void DeactivateInputConfig(const UPlayerMappableInputConfig* Config);

	/** Get an input config with a certain name. If the config doesn't exist then nullptr will be returned. */
	UFUNCTION(BlueprintCallable)
		const UPlayerMappableInputConfig* GetInputConfigByName(FName ConfigName) const;

	/** Get all currently registered input configs */
	const TArray<FLoadedMappableConfigPair>& GetAllRegisteredInputConfigs() const { return RegisteredInputConfigs; }

	/**
	 * Get all registered input configs that match the input type.
	 *
	 * @param Type		The type of config to get, ECommonInputType::Count will include all configs.
	 * @param OutArray	Array to be populated with the current registered input configs that match the type
	 */
	void GetRegisteredInputConfigsOfType(ECommonInputType Type, OUT TArray<FLoadedMappableConfigPair>& OutArray) const;

	/**
	 * Maps the given keyboard setting to the
	 *
	 * @param MappingName	The name of the FPlayerMappableKeyOptions that you would like to change
	 * @param NewKey		The new key to bind this option to
	 */
	void AddOrUpdateCustomKeyboardBindings(const FName MappingName, const FKey NewKey, ULyraLocalPlayer* LocalPlayer);

	const TMap<FName, FKey>& GetCustomPlayerInputConfig() const { return CustomKeyboardConfig; }

private:

	UPROPERTY(Config)
	FName ControllerPlatform;

	UPROPERTY(Config)
	FName ControllerPreset = TEXT("Default");

	/** The name of the current input config that the user has selected. */
	UPROPERTY(Config)
	FName InputConfigName = TEXT("Default");

	/**
	 * Array of currently registered input configs. This is populated by game feature plugins
	 *
	 * @see UGameFeatureAction_AddInputConfig
	 */
	UPROPERTY(VisibleAnywhere)
	TArray<FLoadedMappableConfigPair> RegisteredInputConfigs;

	/** Array of custom key mappings that have been set by the player. Empty by default. */
	UPROPERTY(Config)
	TMap<FName, FKey> CustomKeyboardConfig;
};
