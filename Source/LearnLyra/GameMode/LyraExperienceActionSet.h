// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraExperienceActionSet.generated.h"


class UGameFeatureAction;

/**
 * 
 */
UCLASS(BlueprintType, NotBlueprintable)
class LEARNLYRA_API ULyraExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULyraExperienceActionSet();

	UPROPERTY(EditAnywhere, Instanced, Category = "Actions to Perform")
	TArray<UGameFeatureAction*> Actions;

	UPROPERTY(EditAnywhere, Category = "Feature Dependencies")
	TArray<FString> GameFeaturesToEnable;
};

