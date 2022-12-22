// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "Input/LyraMappableConfigPair.h"
#include "GameFeatureAction_AddInputConfig.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Add Input Config"))
class LEARNLYRA_API UGameFeatureAction_AddInputConfig : public UGameFeatureAction
{
	GENERATED_BODY()
	
public:
	//~UObject UGameFeatureAction
	virtual void OnGameFeatureRegistering() override;
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	virtual void OnGameFeatureUnregistering() override;
	//~End of UGameFeatureAction interface

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif

	/** The player mappable configs to register for user with this config */
	UPROPERTY(EditAnywhere)
	TArray<FMappableConfigPair> InputConfigs;
};
