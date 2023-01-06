// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_ExperienceReady.generated.h"

class AGameStateBase;
class UGameInstance;
class ULyraExperienceDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceReadyAsyncDelegate);

/**
 * 
 */
UCLASS()
class LEARNLYRA_API UAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()

public:
	// Waits for the experience to be determined and loaded
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
	static UAsyncAction_ExperienceReady* WaitForExperienceReady(UObject* WorldContextObject);

	virtual void Activate() override;

public:

	// Called when the experience has been determined and is ready/loaded
	UPROPERTY(BlueprintAssignable)
	FExperienceReadyAsyncDelegate OnReady;

private:
	void Step1_HandleGameStateSet(AGameStateBase* GameState);
	void Step2_ListenToExperienceLoading(AGameStateBase* GameState);
	void Step3_HandleExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience);
	void Step4_BroadcastReady();

	TWeakObjectPtr<UWorld> WorldPtr;
};
