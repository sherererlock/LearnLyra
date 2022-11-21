// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputBaseTypes.h"
#include "GameplayTagContainer.h"

#include "LyraMappableConfigPair.generated.h"

class UPlayerMappableInputConfig;

USTRUCT(BlueprintType)
struct FLoadedMappableConfigPair
{
	GENERATED_BODY()

	FLoadedMappableConfigPair() = default;
	FLoadedMappableConfigPair(const UPlayerMappableInputConfig* InConfig, ECommonInputType InType, const bool InIsActive)
		: Config(InConfig)
		, Type(InType)
		, bIsActive(InIsActive)
	{}

	/** The player mappable input config that should be applied to the Enhanced Input subsystem */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	const UPlayerMappableInputConfig* Config = nullptr;

	/** The type of device that this mapping config should be applied to */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ECommonInputType Type = ECommonInputType::Count;

	/** If this config is currently active. A config is marked as active when it's owning GFA is active */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsActive = false;
};


USTRUCT()
struct FMappableConfigPair
{
	GENERATED_BODY()

	FMappableConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	ECommonInputType Type = ECommonInputType::Count;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer DependentPlatformTraits;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer ExcludedPlatformTraits;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;

	/** Returns true if this config pair can be activated based on the current platform traits and settings. */
	bool CanBeActivated() const;

	/**
	 * Registers the given config mapping with the settings
	 */
	static bool RegisterPair(const FMappableConfigPair& Pair);

	/**
	 * Activates the given config mapping in the settings. This will also register the mapping
	 * if it hasn't been yet.
	 */
	static bool ActivatePair(const FMappableConfigPair& Pair);

	static void DeactivatePair(const FMappableConfigPair& Pair);

	static void UnregisterPair(const FMappableConfigPair& Pair);
};
