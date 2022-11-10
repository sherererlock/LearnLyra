// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraPawnComponent.h"
#include "LyraPawnExtensionComponent.generated.h"

/**
 * 
 */
UCLASS()
class LEARNLYRA_API ULyraPawnExtensionComponent : public ULyraPawnComponent
{
	GENERATED_BODY()
	
public:
	ULyraPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	// Should be called by the owning pawn when the pawn's controller changes.
	void HandleControllerChanged();

	// Should be called by the owning pawn when the input component is setup.
	void SetupPlayerInputComponent();

	// Call this anytime the pawn needs to check if it's ready to be initialized (pawn data assigned, possessed, etc..). 
	bool CheckPawnReadyToInitialize();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Lyra|Pawn", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsPawnReadyToInitialize() const { return bPawnReadyToInitialize; }

	void OnPawnReadyToInitialize_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);

protected:

	// Delegate fired when pawn has everything needed for initialization.
	FSimpleMulticastDelegate OnPawnReadyToInitialize;

protected:
	// True when the pawn has everything needed for initialization.
	int32 bPawnReadyToInitialize : 1;
};
