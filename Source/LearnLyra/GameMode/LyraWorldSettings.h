// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "LyraWorldSettings.generated.h"

class ULyraExperienceDefinition;
/**
 * 
 */
UCLASS()
class LEARNLYRA_API ALyraWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
public:
	ALyraWorldSettings(const FObjectInitializer& ObjectInitializer);

	FPrimaryAssetId GetDefaultGameplayExperience() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	TSoftClassPtr<ULyraExperienceDefinition> DefaultGameplayExperience;
};
